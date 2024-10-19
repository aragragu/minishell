/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 13:19:54 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/19 18:45:40 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sysntax_error_checker(t_garbage **garbage, char **str, t_elem **list)
{
	if (*str && *list)
	{
		if (!check_unclosed_quotes(garbage, str, list))
			return (0);
		if (!check_logical_operators(garbage, list))
			return (0);
		if (!check_pipe_errors(garbage, list))
			return (0);
		if (!check_redirection_errors(garbage, list))
			return (0);
		if (!check_append_and_heredoc(garbage, list))
			return (0);
	}
	return (1);
}

int	check_unclosed_quotes(t_garbage **garbage, char **str, t_elem **list)
{
	if (!has_unclosed_quots(*str))
	{
		my_perror(garbage, list, "Error: Unclosed quotes\n");
		return (0);
	}
	return (1);
}

int	check_logical_operators(t_garbage **garbage, t_elem **list)
{
	if (!has_invalid_logical_operator1(list))
	{
		my_perror(garbage, list,
			"syntax error: logical operators not supported\n");
		return (0);
	}
	if (!has_invalid_logical_operator2(list))
	{
		my_perror(garbage, list,
			"bash : syntax error near unexpected token `)'\n");
		return (0);
	}
	return (1);
}

int	check_pipe_errors(t_garbage **garbage, t_elem **list)
{
	if (!pipe_error(list))
	{
		my_perror(garbage, list,
			"error: syntax error near unexpected token `|'\n");
		return (0);
	}
	return (1);
}

int	check_redirection_errors(t_garbage **garbage, t_elem **list)
{
	if (!has_invalid_redirection_in(list))
	{
		my_perror(garbage, list,
			"Error: syntax error near unexpected token `<'\n");
		return (0);
	}
	if (!has_invalid_redirection_out(list))
	{
		my_perror(garbage, list,
			"Error: syntax error near unexpected token `>'\n");
		return (0);
	}
	return (1);
}

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

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	is_alphanumeric(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
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
				current = current->next->next;
				if (current && (current->type >= PIPE))
					return (0);
			}
			if (current->next && current->next->type >= PIPE)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	has_invalid_redirection_out(t_elem **list)
{
	t_elem	*current;
	t_elem	*pah;

	current = *list;
	if (ft_lstlast(*list)->type == REDIR_OUT)
		return (0);
	while (current)
	{
		if (current && current->type == REDIR_OUT)
		{
			if (current->next && current->next->type == S_PACE)
			{
				pah = current->next->next;
				if (pah && pah->type >= PIPE)
					return (0);
			}
			if (current->next && current->next->type >= PIPE)
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

int	has_invalid_heredoc(t_elem **list)
{
	t_elem	*current;
	t_elem	*pah;

	current = *list;
	if (ft_lstlast(*list)->type == HEREDOC)
		return (0);
	while (current)
	{
		if (current && current->type == HEREDOC)
		{
			if (current->next && current->next->type == S_PACE)
			{
				pah = current->next->next;
				if (pah && pah->type >= PIPE)
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
	t_elem	*pah;

	current = *list;
	if (ft_lstlast(*list)->type == APPEND)
		return (0);
	while (current)
	{
		if (current && current->type == APPEND)
		{
			if (current->next && current->next->type == S_PACE)
			{
				pah = current->next->next;
				if (pah && pah->type >= PIPE)
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
