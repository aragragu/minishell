/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:35:48 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 19:38:26 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	s_append(t_elem **list)
{
	t_elem	*current;
	t_elem	*holder;

	current = *list;
	if (current->next && current->next->type == S_PACE)
	{
		holder = current->next;
		if (holder->next && holder->next->type < S_PACE)
		{
			*list = holder->next;
			holder->next->type = APPEND;
			return ;
		}
	}
	else if (current->next && current->next->type < S_PACE)
	{
		*list = current->next;
		current->next->type = APPEND;
		return ;
	}
}

void	redirection_out_list(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	if (current && current->next)
	{
		if (current->next->type == REDIR_OUT
			&& !ft_strcmp(current->next->content, ">"))
			redirection_out_list2(list);
	}
}

void	redirection_out_list2(t_elem **list)
{
	t_elem	*current;
	t_elem	*red_out;

	current = *list;
	red_out = current->next;
	if (red_out && red_out->next)
	{
		if (red_out->next && red_out->next->type < S_PACE)
		{
			red_out->next->type = REDIR_OUT;
			current->next = red_out->next;
			return ;
		}
		else if (red_out->next->next && red_out->next->type == S_PACE)
		{
			red_out = red_out->next->next;
			if (red_out->type < S_PACE)
			{
				red_out->type = REDIR_OUT;
				current->next = red_out;
				return ;
			}
		}
	}
}

void	redirection_in_list(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	if (current && current->next)
	{
		if (current->next->type == REDIR_IN
			&& !ft_strcmp(current->next->content, "<"))
			redirection_in_list2(list);
	}
}

void	redirection_in_list2(t_elem **list)
{
	t_elem	*current;
	t_elem	*red_out;

	current = *list;
	red_out = current->next;
	if (red_out && red_out->next)
	{
		if (red_out->next && red_out->next->type < S_PACE)
		{
			red_out->next->type = REDIR_IN;
			current->next = red_out->next;
			return ;
		}
		else if (red_out->next->next && red_out->next->type == S_PACE)
		{
			red_out = red_out->next->next;
			if (red_out->type < S_PACE)
			{
				red_out->type = REDIR_IN;
				current->next = red_out;
				return ;
			}
		}
	}
}
