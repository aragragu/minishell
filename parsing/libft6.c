/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:52:48 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/22 22:15:48 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	not_special(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '$' || c == '\"'
		|| c == '\'' || c == ' ' || is_whitespace(c) || c == '('
		|| c == ')' || c == '&')
		return (0);
	return (1);
}

int	is_special_character(char c)
{
	if ((c >= 33 && c <= 47) || (c >= 58 && c <= 64)
		|| (c >= 91 && c <= 96) || (c >= 123 && c <= 126))
		return (1);
	return (0);
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

void	ft_sign(char c, int *sign)
{
	if (c == '"' && *sign == 2)
		*sign = 0;
	else if (c == '\'' && *sign == 1)
		*sign = 0;
	else if (c == '"' && *sign == 0)
		*sign = 2;
	else if (c == '\'' && *sign == 0)
		*sign = 1;
}

t_redir	*ft_lstlast_redi(t_redir *lst)
{
	t_redir	*ptr;
	t_redir	*gtr;

	gtr = NULL;
	if (!lst)
		return (NULL);
	ptr = lst;
	while (ptr)
	{
		if (ptr->type == HEREDOC)
			gtr = ptr;
		ptr = ptr->next;
	}
	return (gtr);
}
