/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 11:29:24 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 19:39:19 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_redirection(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	if (list && ((*list)->type == REDIR_OUT || (*list)->type == REDIR_IN
			|| (*list)->type == APPEND))
		starting_red(list);
	while (current)
	{
		if (current->next && current->next->type == REDIR_OUT)
			redirection_out_list(&current);
		else if (current->next && current->next->type == REDIR_IN)
			redirection_in_list(&current);
		else if (current->next && current->next->type == APPEND)
			append_list(&current);
		current = current->next;
	}
}

int	check_fd_her(t_elem **elem)
{
	t_elem	*list;

	list = *elem;
	while (list)
	{
		if (list->type == HEREDOC && list->fd_here == -2)
			return (1);
		list = list->next;
	}
	return (0);
}

void	starting_red(t_elem **list)
{
	t_elem	*current;

	current = *list;
	if (current && current->type == REDIR_IN)
		s_redir_in(list);
	else if (current && current->type == REDIR_OUT)
		s_redir_out(list);
	else if (current && current->type == APPEND)
		s_append(list);
}

void	s_redir_in(t_elem **list)
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
			holder->next->type = REDIR_IN;
			return ;
		}
	}
	else if (current->next && current->next->type < S_PACE)
	{
		*list = current->next;
		current->next->type = REDIR_IN;
		return ;
	}
}

void	s_redir_out(t_elem **list)
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
			holder->next->type = REDIR_OUT;
			return ;
		}
	}
	else if (current->next && current->next->type < S_PACE)
	{
		*list = current->next;
		current->next->type = REDIR_OUT;
		return ;
	}
}
