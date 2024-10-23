/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 13:19:54 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/23 00:36:17 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
