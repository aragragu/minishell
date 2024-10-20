/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:38:59 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 19:39:48 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_herdoc_line(t_elem **list, t_herdoc *data, t_garbage **garbage)
{
	data->line = readline(">");
	if (!data->line)
	{
		open_herdoc_file(data);
		return (1);
	}
	if (!ft_strcmp((*list)->content, data->line))
	{
		open_herdoc_file(data);
		return (1);
	}
	if (!*data->line)
	{
		free(data->line);
		return (2);
	}
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(data->line));
	return (0);
}

int	write_herdoc_line(t_elem **list, t_herdoc *data, t_garbage **garbage,
t_env **env)
{
	if (!(*list)->ignore[1])
		expand_d_qouts_2(env, &data->line, garbage);
	if (!data->line)
		return (2);
	data->tmp = ft_strjoin(data->line, "\n", garbage);
	if (!data->tmp)
		return (1);
	data->tmp = ft_strjoin(data->buffer, data->tmp, garbage);
	if (!data->tmp)
		return (1);
	data->buffer = data->tmp;
	return (0);
}

void	append_list(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	if (current && current->next)
	{
		if (current->next->type == APPEND
			&& !ft_strcmp(current->next->content, ">>"))
			append_list2(list);
	}
}

void	append_list2(t_elem **list)
{
	t_elem	*current;
	t_elem	*red_out;

	current = *list;
	red_out = current->next;
	if (red_out && red_out->next)
	{
		if (red_out->next && red_out->next->type < S_PACE)
		{
			red_out->next->type = APPEND;
			current->next = red_out->next;
			return ;
		}
		else if (red_out->next->next && red_out->next->type == S_PACE)
		{
			red_out = red_out->next->next;
			if (red_out->type < S_PACE)
			{
				red_out->type = APPEND;
				current->next = red_out;
				return ;
			}
		}
	}
}

void	expand_herdoc(char **str, t_env **env, t_garbage **garbage)
{
	char	*gtr;
	t_env	*list;

	gtr = *str;
	if (*gtr == '$')
	{
		list = *env;
		if (*(gtr + 1) >= '0' && *(gtr + 1) <= '9')
			*str = ft_strdup("", garbage);
		else
		{
			while (list)
			{
				if (!ft_strcmp(list->key, gtr + 1))
				{
					*str = ft_strdup(list->value, garbage);
					return ;
				}
				list = list->next;
			}
			*str = ft_strdup("", garbage);
		}
	}
}
