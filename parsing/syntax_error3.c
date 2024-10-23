/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:14:18 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/23 13:11:28 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_invalid_heredoc(t_elem **list)
{
	t_elem	*current;

	current = *list;
	if (ft_lstlast(*list)->type == HEREDOC)
		return (0);
	while (current)
	{
		if (current && current->type == HEREDOC)
		{
			if (current->next && current->next->type == S_PACE)
			{
				if (current->next->next && current->next->next->type >= PIPE)
					return (0);
			}
			if (current->next && current->next->type >= PIPE)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	has_invalid_append(t_elem **list)
{
	t_elem	*current;

	current = *list;
	if (ft_lstlast(*list)->type == APPEND)
		return (0);
	while (current)
	{
		if (current && current->type == APPEND)
		{
			if (current->next && current->next->type == S_PACE)
			{
				if (current->next->next && current->next->next->type >= PIPE)
					return (0);
			}
			if (current->next && current->next->type >= PIPE)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	has_invalid_logical_operator1(t_elem **list)
{
	t_elem	*current;

	current = *list;
	while (current)
	{
		if (current && current->type == AND)
			return (0);
		current = current->next;
	}
	return (1);
}

int	has_invalid_logical_operator2(t_elem **list)
{
	t_elem	*current;

	current = *list;
	while (current)
	{
		if (current && current->type == OPENING_PARENTHESIS)
			return (0);
		else if (current && current->type == CLOSING_PARENTHESIS)
			return (0);
		current = current->next;
	}
	return (1);
}

void	my_perror(t_garbage **garbage, t_elem **list, char *str)
{
	free_garbage(garbage);
	write(2, str, ft_strlen(str));
	*list = NULL;
	*garbage = NULL;
}
