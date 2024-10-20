/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:44:27 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 19:52:07 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_herdoc_list2(t_elem **token, t_garbage **garbage)
{
	(void )garbage;
	if ((*token)->next->next && ((*token)->next->next->type == VAR
			|| (*token)->next->next->type == S_QOUTS
			|| (*token)->next->next->type == D_QOUTS))
	{
		if ((*token)->next->next->next)
			*token = (*token)->next->next->next;
		else
			return (1);
	}
	return (0);
}

int	handle_herdoc_list3(t_elem **token)
{
	if ((*token)->next->next)
		*token = (*token)->next->next;
	else
		return (1);
	return (0);
}

void	searsh_env(t_env **env, char **str, t_garbage **grb, int end)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	j = 0;
	while (str[j])
	{
		i = 0;
		end = ft_strlen(str[j]);
		while (str[j][i])
		{
			if (str[j][i] && str[j][i] == '=')
			{
				key = ft_substr(str[j], 0, i, grb);
				value = ft_substr(str[j] + i + 1, 0, (end - i - 1), grb);
				if (!ft_strcmp(key, "SHLVL"))
					value = ft_strdup(ft_itoa(ft_atoi2(value) + 1, grb), grb);
				ft_lstadd_back2(env, ft_lstnew2(key, value, grb));
				break ;
			}
			i++;
		}
		j++;
	}
}

void	fill_env(t_env **env, char **str, t_garbage **garbage)
{
	int		end;

	end = 0;
	if (!str || !str[0])
	{
		*env = ft_lstnewww(ft_strduppp("PWD"), getcwd(NULL, 0));
		if (!*env)
			return ;
		ft_lstadd_backkk(env, ft_lstnewww(ft_strduppp("SHLVL"),
				ft_strduppp("1")));
		ft_lstadd_backkk(env, ft_lstnewww(ft_strduppp("_"),
				ft_strduppp("/usr/bin/env")));
		ft_lstadd_backkk(env, ft_lstnewww(ft_strduppp("OLDPWD"), NULL));
		return ;
	}
	searsh_env(env, str, garbage, end);
}

void	expand_var2(t_elem *node, char *gtr, t_garbage **grb, t_env *env)
{
	int		flag;

	flag = 0;
	while (env)
	{
		if (!ft_strcmp(env->key, gtr + 1))
		{
			if (env->value)
			{
				node->content = ft_strtrim(ft_strdup(env->value, grb),
						" \t\n\v\f\r", grb);
				flag = 1;
				break ;
			}
			else
				break ;
		}
		env = env->next;
	}
	if (!flag)
		node->content = NULL;
}
