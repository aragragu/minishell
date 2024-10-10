/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:33:35 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/10 22:27:49 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_cat(char *str, int len, int flag)
{
	int		i;
	char	*strn;

	if (flag)
		strn = malloc(ft_strlen(str) - len + 1);
	else
		strn = malloc(len + 1);
	if (!str || !strn)
		return (NULL);
	i = 0;
	if (flag)
	{
		while (len < ft_strlen(str))
			strn[i++] = str[len++];
	}
	else
	{
		while (i < len)
		{
			strn[i] = str[i];
			i++;
		}
	}
	strn[i] = '\0';
	return (strn);
}

void	ft_env(t_var *var)
{
	t_env	*tmp;

	tmp = var->env;
	if (var->list->argc[1])
	{
		printf("env: too many arguments\n");
		var->exit_num = 127;
		return ;
	}
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	var->exit_num = 0;
}
