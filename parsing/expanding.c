/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:15:38 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/21 20:17:18 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	len(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13) || str[i] == '+')
		i++;
	if ((str[i] >= '0' && str[i] <= '9') || (str[i] == '-' && str[i] == '0'))
		i++;
	return (i);
}

static int	over_flow(int sign, const char *str)
{
	int	i;

	i = 0;
	while (str[i] == '+' || str[i] == '-')
		i++;
	if (len(str) > 20 && sign == -1)
		return (0);
	if (len(str) > 19 && sign == 1)
		return (-1);
	return (1);
}

int	ft_atoi2(char *str)
{
	size_t	i;
	int		n;
	size_t	result;

	i = 0;
	n = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			n = -n;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - 48);
		if (over_flow(n, str) != 1)
			return (over_flow(n, str));
		i++;
	}
	return (result * n);
}

void	expand_var_list(t_elem **list, t_var container, t_garbage **garbage)
{
	t_elem	*token;

	if (!list || !*list)
		return ;
	token = *list;
	while (token)
	{
		if (token && token->type == HEREDOC)
		{
			if (handle_herdoc_list(&token, garbage) == 1)
				break ;
		}
		if (token && token->type == VAR)
			expand_var(list, token, &container.env, garbage);
		else if (token && token->type == EXIT_STATUS)
			token->content = ft_itoa(g_es(0, 1), garbage);
		else if (token && token->type == D_QOUTS)
			expand_d_qouts(&container.env, &token->content, garbage);
		else if (token && token->type == TILDE)
			token->content = getenv("HOME");
		token = token->next;
	}
}

int	handle_herdoc_list(t_elem **token, t_garbage **garbage)
{
	int	i;

	if (token && *token && (*token)->type == HEREDOC)
	{
		if ((*token)->next && (*token)->next->type == S_PACE)
		{
			i = handle_herdoc_list2(token, garbage);
			if (i)
				return (1);
		}
		else
		{
			if ((*token)->next && ((*token)->next->type == VAR
					|| (*token)->next->type == S_QOUTS
					|| (*token)->next->type == D_QOUTS))
			{
				i = handle_herdoc_list3(token);
				if (i)
					return (1);
			}
		}
	}
	return (0);
}
