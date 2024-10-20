/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 10:44:37 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 19:34:21 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	read_input(char **env)
{
	struct termios	original_termios;
	t_var			var;
	char			*input;
	int				i;

	input = NULL;
	initialize_variables(&var, env);
	tcgetattr(STDIN_FILENO, &original_termios);
	while (1)
	{
		g_sig = 0;
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, signal_handler);
		input = readline("➜ minishell💀$ ");
		i = fill_linked_list(input, &var);
		if (i == 1)
			break ;
		else if (i == 2)
			continue ;
		if (g_sig == 0)
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

int	fill_and_check(char *input, t_var *var)
{
	if (!input)
		return (1);
	if (!*input)
	{
		free(input);
		return (2);
	}
	ft_lstadd_back_garbage(&var->garbage, ft_lstnew_garbage(input));
	add_history(input);
	token_input(&input, var);
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
	return (0);
}

void	ft_heredoc(t_var *data)
{
	char	*buff;
	t_elem	*tmp;
	t_elem	*list;

	tmp = NULL;
	list = data->linked_list;
	buff = NULL;
	edit_linked_list(data, tmp, list, buff);
	remove_delemiter(&data->linked_list, &data->garbage);
}

void	edit_linked_list(t_var *data, t_elem *tmp, t_elem *list, char *buff)
{
	t_elem	*ptr;
	int		flag;

	flag = 0;
	ptr = list;
	while (list)
	{
		buff = ft_strdup("", &data->garbage);
		if (list->type == HEREDOC && !check_fd_her(&ptr))
		{
			tmp = list->next;
			if (tmp && tmp->type == S_PACE)
			{
				tmp->ignore[0] = 1;
				tmp = tmp->next;
			}
			while (tmp && tmp->type < S_PACE)
			{
				if (tmp->type == S_QOUTS || tmp->type == D_QOUTS)
					flag = 1;
				tmp->ignore[0] = 1;
				buff = ft_strjoin(buff, tmp->content, &data->garbage);
				tmp = tmp->next;
			}
			list->content = buff;
			if (flag)
				list->ignore[1] = 1;
			open_herdoc(&list, &data->env, &data->garbage);
		}
		list = list->next;
	}
}
