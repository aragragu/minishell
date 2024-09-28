/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:33:35 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/28 15:54:06 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	norm_init_env(t_env **envr, char **env)
{
	char	*key;
	char	*val;
	int		i;
	int		j;

	i = 1;
	j = 0;
	key = NULL;
	val = NULL;
	while (env[i])
	{
		j = -1;
		while (env[i][++j])
		{
			if (env[i][j] == '=')
			{
				key = ft_cat(env[i], j, 0);
				val = ft_cat(env[i], j + 1, 1);
				ft_lstadd_backkk(envr, ft_lstnewww(key, val));
				break ;
			}
		}
		if (env[i][j] != '=')
        {
            printf("env: %s: No such file or directory\n", env[i]);
        }
		i++;
	}
}

void	init_env(t_env **envr, char **env)
{
	int		i;
	char	*key;
	char	*val;

	i = -1;
	key = NULL;
	val = NULL;
	*envr = ft_lstnewww(ft_strduppp("PWD"), ft_strduppp("/Users/ykasmi/Desktop/parss/parsing")); // protection
	if (!env || !env[0])
	{
		ft_lstadd_backkk(envr, ft_lstnewww(ft_strduppp("SHLVL"), ft_strduppp("1")));
		ft_lstadd_backkk(envr, ft_lstnewww(ft_strduppp("_"), ft_strduppp("/usr/bin/env")));
		return ;
	}
	while (env[0][++i])
	{
		if (env[0][i] == '=')
		{
			key = ft_cat(env[0], i, 0);
			val = ft_cat(env[0], i + 1, 1);
			break ;
		}
	}
	*envr = ft_lstnewww(key, val); // protection
	norm_init_env(envr, env);
}

char	*ft_cat(char *str, int len, int flag)
{
	int		i;
	char	*strn;

	if (flag)
		strn = malloc(ft_strlen(str) - len + 1); // protection
	else
		strn = malloc(len + 1); // protection
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

	// Check if env is properly initialized
	if (!var || !var->env)
	{
		fprintf(stderr, "Error: env not initialized.\n");
		return;
	}

	// Access environment variables list
	tmp = var->env;

	// If any argument is passed with 'env', handle it with a proper message
	if (var->list->argc[1])
	{
		printf("env: %s: No such file or directory\n", var->list->argc[1]);
		return;
	}

	// Iterate through the environment linked list and print key=value pairs
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}