/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 10:44:37 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/22 12:31:35 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_variables(t_var *var, char **env)
{
	rl_catch_signals = 0;
	var->env = NULL;
	var->list = NULL;
	g_es(0, 0);
	var->garbage = NULL;
	var->garb = NULL;
	var->linked_list = NULL;
	var->path = ft_strduppp(_PATH_STDPATH);
	fill_env(&var->env, env, &var->garb);
}

int	fill_and_check(char *input, t_var *var)
{
	if (!input)
		return (1);
	if (!*input)
		return (2);
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
	t_elem	*list;

	list = data->linked_list;
	edit_linked_list(data, list);
	remove_delemiter(&data->linked_list, &data->garbage);
}

void	edit_linked_list(t_var *data, t_elem *list)
{
	t_elem	*ptr;

	ptr = list;
	while (list)
	{
		if (list->type == HEREDOC && !check_fd_her(&ptr))
			handle_heredoc(list, data);
		list = list->next;
	}
}
