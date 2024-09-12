/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:31:11 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/12 15:54:06 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd_upd_old(t_env **env, char *key, char *val)
{
	t_env	*current;
	t_env	*new_node;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strduppp(val);
			return ;
		}
		current = current->next;
	}
	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node)
	{
		new_node->key = ft_strduppp(key);
		new_node->value = ft_strduppp(val);
		new_node->next = NULL;
		new_node->flag = 1;
		ft_lstadd_backkk(env, new_node);
	}
}

void	ft_cd(t_var *var)
{
	char	cwd[PATH_MAX];
	char	old_pwd[PATH_MAX];
	char	*home;
	// char	**envp;

	// store_env(var->env, &envp);
	if (var->list->argc[1] != NULL && !ft_strcmp(var->list->argc[1], ".") \
		&& getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		printf("minishell: cd: No such file or directory\n");
		return ;
	}
	else if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		if (chdir("..") != 0)
		{
			printf("minishell: cd: No such file or directory\n");
			return ;
		}
	}
	else if (var->list->argc[1] == NULL)
	{
		home = ft_getenv(var->env, "HOME");
		if (home == NULL)
		{
			printf("minishell: cd: HOME not set\n");
			return ;
		}
		if (chdir(home) != 0)
		{
			printf("minishell: cd: %s: No such file or directory\n", home);
			return ;
		}
	}
	else
	{
		if (chdir(var->list->argc[1]) != 0)
		{
			printf("cd: %s: Not a directory\n", var->list->argc[1]);
			return ;
		}
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		pwd_upd_old(&var->env, "OLDPWD", old_pwd);
		pwd_upd_old(&var->env, "PWD", cwd);
	}
	else
		perror("getcwd");
}
