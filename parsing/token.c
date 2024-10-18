/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 10:44:37 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/18 04:09:37 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_es(int stat, int flag)
{
	static int exit_status;

	if (flag == 0)
	{
		exit_status = stat;
	}
	return (exit_status);
}

void	signal_hand_sig_qui(int sig)
{
	if (sig == SIGQUIT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		ft_fprintf(1, "Quit: 3\n");
		g_es(131, 0);
	}
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		if(!waitpid(-1, NULL, WNOHANG))
			return;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_es(1, 0);
	}
	if (sig == SIGQUIT)
		return ;
}

void	initialize_variables(t_var *var, char **env)
{
	var->env = NULL;
	var->list = NULL;
	g_es(0, 0);
	var->garbage = NULL;
	var->garb = NULL;
	var->linked_list = NULL;
	var->path = ft_strduppp(_PATH_STDPATH);
	fill_env(&var->env, env, &var->garb);
	rl_catch_signals = 0;
	
}

int	fill_linked_list(char *input, int *p, t_var *var)
{
	(void)p;
	if (!input)
		return (1);
	if (!*input)
	{
		free(input);
		return (2);
	}
	ft_lstadd_back_garbage(&var->garbage, ft_lstnew_garbage(input));
	add_history(input);
	var->linked_list = token_input(&var->linked_list, &input, var, &var->garbage);
	if (!var->linked_list)
	{
		free_garbage(&var->garbage);
		return (2);
	}
	if (!sysntax_error_checker(&var->garbage, &input, &var->linked_list))
	{
		free_garbage(&var->garbage);
		g_es(258, 0);
		var->linked_list = NULL;
		var->garbage = NULL;
		var->list = NULL;
		return (2);
	}
	expand_var_list(&var->linked_list, *var, &var->garbage);
	concatination(&var->linked_list, &var->garbage);
	handle_redirection(&var->linked_list, &var->env, &var->garbage);
	if (check_fd_her(&var->linked_list))
	{
		free_garbage(&var->garbage);
		g_es(1, 0);
		var->linked_list = NULL;
		var->garbage = NULL;
		var->list = NULL;
		return (2);
	}
	if (g_sig == 2)
	{
		free_garbage(&var->garbage);
		g_es(1, 0);
		var->linked_list = NULL;
		var->garbage = NULL;
		var->list = NULL;
		return (2);
	}
	import_data(&var->list, &var->linked_list, &var->garbage);
	// print_list(&var->linked_list);
	// print_cmd(var->list);
	// puts("paaaaaaaah");
	// print_list(&var->linked_list);
	// print_cmd(var->list);
	return (0);
}

void	read_input(char **env)
{
	t_var	var;
	char	*input;
	int		i;
	int p[2];
	struct termios original_termios;

	input = NULL;
	initialize_variables(&var, env);
	tcgetattr(STDIN_FILENO, &original_termios);
	while (1)
	{
		g_sig = 0;
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, signal_handler);
		input = readline("➜ minishell💀$ ");
		i = fill_linked_list(input, p, &var);
		// puts("allllllo");
		if (i == 1)
			break ;
		else if (i == 2)
			continue ;
		if(g_sig == 0 && var.list != NULL)
		{
			execution(&var);
			tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
		}
		free_garbage(&var.garbage);
		var.linked_list = NULL;
		var.garbage = NULL;
		var.list = NULL;
	}
	free_garbage(&var.garb);
}

int	not_special(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '$' || c == '\"' || \
	c == '\'' || c == ' ')
		return (0);
	return (1);
}

t_elem	*token_input(t_elem **list, char **in, t_var *var, t_garbage **garbage)
{
	int		i;
	char	*input;

	i = 0;
	input = ft_strtrim(*in, " \t\n\v\f\r", garbage);
	while (input && input[i])
	{
		if (input[i] && is_witheS_PACE(input[i]))
		{
			while (input[i] && is_witheS_PACE(input[i]))
				i++;
			i--;
			ft_lstadd_back(list, ft_lstnew(ft_strdup(" ", garbage) \
			, S_PACE, garbage));
		}
		if (input[i] == '#')
			ft_lstadd_back(list, ft_lstnew(ft_strdup("#", garbage), HASH, garbage));
		else if (input[i] == '>' && input[i + 1] == '>')
			ft_lstadd_back(list, ft_lstnew(ft_strdup(">>", garbage) \
			, APPEND, garbage));
		else if (input[i] == '<' && input[i + 1] == '<')
			ft_lstadd_back(list, ft_lstnew(ft_strdup("<<", garbage) \
			, HEREDOC, garbage));
		else if (input[i] == '<')
			ft_lstadd_back(list, ft_lstnew(ft_strdup("<", garbage) \
			, REDIR_IN, garbage));
		else if (input[i] == '>')
			ft_lstadd_back(list, ft_lstnew(ft_strdup(">", garbage) \
			, REDIR_OUT, garbage));
		else if (input[i] == '$' && (input[i + 1] == '\"' || input[i + 1] == '\''))
		{
			i++;
			continue ;
		}
		else if (input[i] == '$' && input[i + 1] == '$')
			ft_lstadd_back(list, ft_lstnew(ft_strdup("$$", garbage) \
			, DOUBLE_DLR, garbage));
		else
			token_input_1(list, input, i, var);
		i += ft_strlen(ft_lstlast(*list)->content);
		// printf("[%s]", ft_lstlast(*list)->content);
	}
	return (*list);
}

void	token_input_1(t_elem **list, char *input, int i, t_var *var)
{
	if (input[i] == '$' && input[i + 1] == '?')
		ft_lstadd_back(list, ft_lstnew(ft_strdup("$?", &var->garbage) \
		, EXIT_STATUS, &var->garbage));
	else if (input[i] == '$')
		is_a_var(list, input, i, &var->garbage);
	if (input[i] == '\"')
		is_a_quot(list, input, i, &var->garbage);
	else if (input[i] == '\'')
		is_a_squot(list, input, i, &var->garbage);
	else if (input[i] == '|' && (g_es(0, 0), 1))
		ft_lstadd_back(list, ft_lstnew(ft_strdup("|", &var->garbage), PIPE, &var->garbage));
	else if (input[i] == '(')
		ft_lstadd_back(list, ft_lstnew(ft_strdup("(", &var->garbage) \
		, OPENING_PARENTHESIS, &var->garbage));
	else if (input[i] == ')')
		ft_lstadd_back(list, ft_lstnew(ft_strdup(")", &var->garbage) \
		, CLOSING_PARENTHESIS, &var->garbage));
	else if (not_special(input[i]))
		is_a_word(list, input, i, &var->garbage);
}


void	is_a_word(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*word;

	len = 0;
	while (input[index + len] && not_special(input[index + len]) && \
	!is_witheS_PACE(input[index + len]))
		len++;
	word = ft_substr(input, index, len, garbage);
	ft_lstadd_back(list, ft_lstnew(word, WORD, garbage));
}

void	is_a_quot(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*str;

	len = 0;
	while (input[index + 1 + len] && input[index + 1 + len] != '\"')
		len++;
	str = ft_substr(input, index, len + 2, garbage);
	ft_lstadd_back(list, ft_lstnew(str, D_QOUTS, garbage));
}

void	is_a_squot(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*str;

	len = 0;
	while (input[index + 1 + len] && input[index + 1 + len] != '\'')
		len++;
	str = ft_substr(input, index, len + 2, garbage);
	ft_lstadd_back(list, ft_lstnew(str, S_QOUTS, garbage));
}

void	is_a_var(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*str;

	len = 0;
	if (input[index + 1] >= '0' && input[index + 1] <= '9')
	{
		str = ft_substr(input, index, 2, garbage);
		ft_lstadd_back(list, ft_lstnew(str, VAR, garbage));
		return ;
	}
	else
	{
		while (input[index + 1 + len] && (is_alphanumeric(input[index + 1 + len]) \
			|| input[index + 1 + len] == '_'))
			len++;
		str = ft_substr(input, index, len + 1, garbage);
		if (ft_strlen(str) == 1)
			ft_lstadd_back(list, ft_lstnew(str, WORD, garbage));
		else
			ft_lstadd_back(list, ft_lstnew(str, VAR, garbage));
	}
}

void	is_a_string(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*word;

	len = 0;
	while (input[index + len] && input[index + len] != '$' && \
	!is_witheS_PACE(input[index + len]))
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
