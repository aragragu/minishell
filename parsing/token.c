/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 10:44:37 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/07 20:40:47 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
	}
	if (sig == SIGQUIT)
		return ;
}
void read_input(char **env)
{
	t_elem *list = NULL;
	t_garbage *garbage = NULL;
	t_garbage *garb = NULL;
	t_var var;
	var.env = NULL;
	var.list = NULL;
	var.exit_num = 0;
	char *input;
	// int p[2];
	var.path = ft_strduppp(_PATH_STDPATH);
	fill_env(&var.env, env, &garb);
	rl_catch_signals = 0;
	while (1)
	{
		// p[0] = dup(STDIN_?_FILENO);
		input = readline("➜ minishell💀$ ");
		if (!input)
			break;
		if (!*input)
		{
			free(input);
			continue;
		}
		ft_lstadd_back_garbage(&garbage, ft_lstnew_garbage(input));
		add_history(input);
		list = token_input(&list, &input, &var, &garbage);
		if (!list)
			continue;
		if (!sysntax_error_checker(&garbage, input, &list))
		{
			var.exit_num = 258;
			continue;
		}
		expand_var_list(&list, var, &garbage);
		concatination(&list, &garbage);
		handle_redirection(&list, &var.env, &garbage);
		import_data(&var.list, &list, &garbage);
		// print_list(&list);
		// print_cmd(var.list);
		if (list)
			execution(&var);
		free_garbage(&garbage);
		list = NULL;
		garbage = NULL;
		var.list = NULL;
		// dup2(p[0], STDIN_FILENO);
		// dup2(p[1], STDOUT_FILENO);
		// close(p[0]);
		// close(p[1]);
		// var.exit_num = 0;
	}
	free_garbage(&garb);
}


int not_special(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '$' || c == '\"' || c == '\'' || c == ' ')
		return (0);
	return (1);
}

t_elem *token_input(t_elem **list, char **in,t_var *var, t_garbage **garbage)
{
    int i = 0;
    char *input;
    input = ft_strtrim(*in, " \t\n\v\f\r", garbage);
    while (input && input[i])
    {
        if (input[i] && is_witheS_PACE(input[i]))
        {
            while (input[i] && is_witheS_PACE(input[i]))
                i++;
            i--;
            ft_lstadd_back(list, ft_lstnew(ft_strdup(" ", garbage), S_PACE, garbage));
        }
        else if (input[i] == '>' && input[i + 1] == '>')
            ft_lstadd_back(list, ft_lstnew(ft_strdup(">>", garbage), APPEND, garbage));
        else if (input[i] == '<' && input[i + 1] == '<')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("<<", garbage), HEREDOC, garbage));
        else if (input[i] == '<')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("<", garbage), REDIR_IN, garbage));
        else if (input[i] == '>')
            ft_lstadd_back(list, ft_lstnew(ft_strdup(">", garbage), REDIR_OUT, garbage));
        else if (input[i] == '$' && input[i + 1] == '$')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("$$", garbage), DOUBLE_DLR, garbage));
        else if (input[i] == '$' && input[i + 1] == '?')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("$?", garbage), EXIT_STATUS, garbage));
        else if (input[i] == '$' && (input[i + 1] == '\"' || input[i + 1] == '\''))
        {
            i++;
            continue;
        }
        else if (input[i] == '$')
            is_a_var(list, input, i, garbage);
        else if (input[i] == '|' && (var->exit_num = 0, 1))
            ft_lstadd_back(list, ft_lstnew(ft_strdup("|", garbage), PIPE, garbage));
        else if (input[i] == '\"')
            is_a_quot(list, input, i, garbage);
        else if (input[i] == '\'')
            is_a_squot(list, input, i, garbage);
        else if (input[i] == '(')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("(", garbage), OPENING_PARENTHESIS, garbage));
        else if (input[i] == ')')
            ft_lstadd_back(list, ft_lstnew(ft_strdup(")", garbage), CLOSING_PARENTHESIS, garbage));
        else if (input[i] == '#')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("#", garbage), HASH, garbage));
        else if (not_special(input[i]))
            is_a_word(list, input, i, garbage);
        i += ft_strlen(ft_lstlast(*list)->content);
    }
    return (*list);
}

void is_a_word(t_elem **list, char *input, int index, t_garbage **garbage)
{
	char *word;
	int len = 0;
	while (input[index + len] && not_special(input[index + len]) && !is_witheS_PACE(input[index + len]))
		len++;
	word = ft_substr(input, index, len, garbage);
	ft_lstadd_back(list, ft_lstnew(word, WORD, garbage));
}

void is_a_quot(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int len = 0;
	char *str;
	while (input[index + 1 + len] && input[index + 1 + len] != '\"')
		len++;
	str = ft_substr(input, index, len + 2, garbage);
	ft_lstadd_back(list, ft_lstnew(str, D_QOUTS, garbage));
}

void is_a_squot(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int len = 0;
	char *str;
	while (input[index + 1 + len] && input[index + 1 + len] != '\'')
		len++;
	str = ft_substr(input, index, len + 2, garbage);
	ft_lstadd_back(list, ft_lstnew(str, S_QOUTS, garbage));
}

void is_a_var(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int len = 0;
	char *str;
	if (input[index + 1] >= '0' && input[index + 1] <= '9')
	{
		str = ft_substr(input, index, 2, garbage);
		ft_lstadd_back(list, ft_lstnew(str, VAR, garbage));
		return;
	}
	else
	{
		while (input[index + 1 + len] && (is_alphanumeric(input[index + 1 + len]) || input[index + 1 + len] == '_'))
			len++;
		str = ft_substr(input, index, len + 1, garbage);
		if (ft_strlen(str) == 1)
			ft_lstadd_back(list, ft_lstnew(str, WORD, garbage));
		else
			ft_lstadd_back(list, ft_lstnew(str, VAR, garbage));
	}
}

void is_a_string(t_elem **list, char *input, int index, t_garbage **garbage)
{
	char *word;
	int len = 0;
	while (input[index + len] && input[index + len] != '$' && !is_witheS_PACE(input[index + len]))
		len++;
	word = ft_substr(input, index, len, garbage);
	ft_lstadd_back(list, ft_lstnew(word, WORD, garbage));
}

void edit_list(t_elem *list, t_garbage **garbage)
{
	if (list && list->type == D_QOUTS)
		list->content = ft_strtrim(list->content, "\"", garbage);
	else if (list && list->type == S_QOUTS)
		list->content = ft_strtrim(list->content, "\'", garbage);
}

int is_special_character(char c)
{
	if ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126))
		return (1);
	return (0);
}
