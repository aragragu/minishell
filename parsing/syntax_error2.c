/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:12:50 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/23 00:33:13 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_append_and_heredoc(t_garbage **garbage, t_elem **list)
{
	if (!has_invalid_heredoc(list))
	{
		my_perror(garbage, list,
			"Error: syntax error near unexpected token `<<'\n");
		return (0);
	}
	if (!has_invalid_append(list))
	{
		my_perror(garbage, list,
			"Error: syntax error near unexpected token `>>'\n");
		return (0);
	}
	return (1);
}

int	has_unclosed_quots(char *str)
{
	int	i;
	int	sign;

	i = 0;
	sign = 0;
	while (str && str[i])
	{
		while (str[i] && is_whitespace(str[i]))
			i++;
		if (str[i])
		{
			ft_sign(str[i], &sign);
			i++;
		}
	}
	if (sign == 1 || sign == 2)
		return (0);
	return (1);
}

int	has_invalid_redirection_in(t_elem **list)
{
	t_elem	*current;

	current = *list;
	if (ft_lstlast(*list)->type == REDIR_IN)
		return (0);
	while (current)
	{
		if (current && current->type == REDIR_IN)
		{
			if (current->next && current->next->type == S_PACE)
			{
				if (current->next->next && current->next->next->type >= PIPE)
					return (0);
			}
			else if (current->next && current->next->type >= PIPE)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	has_invalid_redirection_out(t_elem **list)
{
	t_elem	*current;

	current = *list;
	if (ft_lstlast(*list)->type == REDIR_OUT)
		return (0);
	while (current)
	{
		if (current && current->type == REDIR_OUT)
		{
			if (current->next && current->next->type == S_PACE)
			{
				if (current->next->next && current->next->next->type >= PIPE)
					return (0);
			}
			else if (current->next && current->next->type >= PIPE)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	pipe_error(t_elem **head)
{
	t_elem	*list;
	t_elem	*pah;

	list = *head;
	if (list->type == PIPE)
		return (0);
	else if (ft_lstlast(list)->type == PIPE)
		return (0);
	while (list)
	{
		if (list && list->type == PIPE)
		{
			if (list->next && list->next->type == S_PACE)
			{
				pah = list->next->next;
				if (pah && pah->type == PIPE)
					return (0);
			}
			if (list->next && list->next->type == PIPE)
				return (0);
		}
		list = list->next;
	}
	return (1);
}
