/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 10:44:37 by aragragu          #+#    #+#             */
/*   Updated: 2024/09/26 15:17:25 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calculate_cmd(t_var *var)
{
	int num_cmd = 0;
	t_cmd *list = var->list;
	while (list)
	{
		num_cmd++;
		list = list->next;
	}
	return(num_cmd);
}

void	execution(t_var *var)
{
	if (check_valid_path(var->list->cmd, var) || check_builtins(var->list->cmd) \
		|| access(var->list->cmd, X_OK) == 0 || contains_red(var) == 0)
	{
		int num_cmd = calculate_cmd(var);
		
		if ((num_cmd > 1 || access(var->list->cmd, X_OK) == 0 || contains_red(var) == 0))
			execute_pipe(num_cmd, var);
		else
		{	
			if (check_builtins(var->list->cmd))
				ft_builtins(var, var->list->cmd, &var->list);
			else
				ft_exc(var);
		}
	}
	else if (ft_strchr(var->list->cmd, '/') != NULL)
		ft_exc2(var);
	else
		fprintf(stderr, "minishell: %s: command not found\n", var->list->argc[0]);
}

int is_directory(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) != 0)
        return 0;
    return S_ISDIR(path_stat.st_mode);
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
	int p[2];
	fill_env(&var.env, env, &garb);
	p[0] = dup(STDIN_FILENO);
	p[1] = dup(STDOUT_FILENO);
	while (1)
	{
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
		list = token_input(&list, &input, &garbage);
		// print_list(&list);
		if (!list)
			continue;
		if (!sysntax_error_checker(&garbage, input, &list))
		{
			var.exit_num = 258;
			continue;
		}
		expand_var_list(&list, &var.env, &garbage);
		handle_redirection(&list, &var.env, &garbage);
		concatination(&list, &garbage);
		import_data(&var.list, &list, &garbage);
		// print_cmd(var.list);
			execution(&var);
		// else
		// 	execution(NULL, &var); 
		// else 
		// 	handle_redirection2(&var);
		// if (ft_strcmp(input, "env") == 0)
		// 	ft_env(&var.env);
		
		// if (check_valid_path(var.list->cmd, &var) || check_builtins(var.list->cmd) || access(var.list->cmd, X_OK) == 0)
		// {
		// 	int num_cmds = calculate_num_cmds(input);
		// 	if (contains_pipe(input))
		// 		execute_pipe(input, num_cmds, &var);
		// 	else
		// 	{	
		// 		if (check_builtins(var.list->cmd))
		// 			ft_builtins(&var, var.list->cmd, &var.list);
		// 		else
		// 			ft_exc(&var);
		// 	}
		// }
		// else if (ft_strchr(var.list->cmd, '/') != NULL)
		// 	ft_exc2(&var);
		// else
		// 	fprintf(stderr, "minishell: %s: command not found\n", var.list->argc[0]);
		free_garbage(&garbage);
		list = NULL;
		garbage = NULL;
		var.list = NULL;
		dup2(p[0], STDIN_FILENO);
		dup2(p[1], STDOUT_FILENO);
	}
	free_garbage(&garb);
}

int not_special(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '$' || c == '\"' || c == '\'' || c == ' ')
		return (0);
	return (1);
}

t_elem *token_input(t_elem **list, char **in, t_garbage **garbage)
{
    int i = 0;
    char *input;
    // ft_lstadd_back(list, ft_lstnew(ft_strdup("", garbage), NULL_TOKEN, garbage));
    input = ft_strtrim(*in, " \t\n\v\f\r", garbage);
    while (input && input[i])
    {
        if (input[i] && is_withespace(input[i]))
        {
            while (input[i] && is_withespace(input[i]))
                i++;
            i--;
            ft_lstadd_back(list, ft_lstnew(ft_strdup(" ", garbage), SPACE, garbage));
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
        else if (input[i] == '$' && (input[i + 1] == '\"' || input[i + 1] == '\''))
        {
            i++;
            continue;
        }
        else if (input[i] == '$')
            is_a_var(list, input, i, garbage);
        else if (input[i] == '|')
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
    // ft_lstadd_back(list, ft_lstnew(ft_strdup("", garbage), NULL_TOKEN, garbage));
    return (*list);
}

void is_a_word(t_elem **list, char *input, int index, t_garbage **garbage)
{
	char *word;
	int len = 0;
	while (input[index + len] && not_special(input[index + len]) && !is_withespace(input[index + len]))
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
		while (input[index + 1 + len] && (is_alphanumeric(input[index + 1 + len]) || input[index + 1 + len] == '_' || input[index + 1 + len] == '?'))
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
	while (input[index + len] && input[index + len] != '$' && !is_withespace(input[index + len]))
		len++;
	word = ft_substr(input, index, len, garbage);
	ft_lstadd_back(list, ft_lstnew(word, WORD, garbage));
}

void edit_list(t_elem *list, t_garbage **garbage)
{
	char *str;
	if (list && list->type == D_QOUTS)
	{
		str = ft_strtrim(list->content, "\"", garbage);
		list->content = str;
	}
	if (list && list->type == S_QOUTS)
	{
		str = ft_strtrim(list->content, "\'", garbage);
		list->content = str;
	}
}

int is_special_character(char c)
{
	if ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126))
		return (1);
	return (0);
}



// "void	execute_pipe(char *input, int num_cmds, t_var *var)
// {
// 	int		pipefd[2];
// 	int		prev_fd = STDIN_FILENO;
// 	char	*input_copy;
// 	char	**envp;
// 	char	*cmd;
// 	char	**args;
// 	char	*cmd_path;
// 	int		i = 0;
// 	pid_t pid;

// 	input_copy = strdup(input);
// 	store_env(var->env, &envp);
// 	cmd = strtok(input_copy, "|");
// 	while (i < num_cmds)
// 	{
// 		pipe(pipefd);
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			dup2(prev_fd, 0);
// 			if (i < num_cmds - 1)
// 			{	
// 				close(pipefd[0]);
// 				dup2(pipefd[1], 1);
// 				close(pipefd[1]);
// 			}
// 			args = parse_command(cmd);
// 			cmd_path = excu_in_path(args[0], var);
// 			if (cmd_path)
// 			{
// 				if (execve(cmd_path, args, envp) == -1)
// 					fprintf(stderr, "minishell: command not found: %s\n", "");
// 				free(cmd_path);
// 			}
// 		}
// 		close(pipefd[1]);
// 		prev_fd = pipefd[0];
// 		cmd = strtok(NULL, "|");
// 		i++;
// 	}
// 	close(prev_fd);
// 	while (wait(NULL) > 0);
// 	free(input_copy);
// }"