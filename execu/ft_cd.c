/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:31:11 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/22 12:19:24 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd_upd_old(t_var *var, char *key, char *val)
{
	t_env	*current;
	t_env	*new_node;

	current = var->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value =NULL;
			current->value = ft_strdup(val, &var->garb);
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
		ft_lstadd_backkk(&var->env, new_node);
	}
}

void	error_fun_cd(t_var *var, char *home, int flag)
{
	if (flag == 0)
	{
		perror(var->list->argc[1]);
		g_es(1, 0);
	}
	else if (flag == 1)
	{
		ft_fprintf(2, "minishell: cd: HOME not set\n");
		g_es(1, 0);
	}
	else if (flag == 2)
	{
		ft_fprintf(2, "cd:%s:No such file or directory\n", home);
		g_es(1, 0);
	}
}

void	ft_cd_handle_special_cases(t_var *var, char *old_pwd, char *home)
{
	if (var->list->argc[1] == NULL)
	{
		home = ft_getenv(var->env, "HOME");
		if (home == NULL)
		{
			error_fun_cd(var, home, 1);
			return ;
		}
		if (chdir(home) != 0)
		{
			error_fun_cd(var, home, 2);
			return ;
		}
	}
	else if (chdir(var->list->argc[1]) != 0)
	{
		error_fun_cd(var, home, 0);
		return ;
	}
	pwd_upd_old(var, "OLDPWD", old_pwd);
}

void	ft_cd(t_var *var)
{
	char	cwd[PATH_MAX];
	char	old_pwd[PATH_MAX];

	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		if (chdir(var->list->argc[1]) != 0)
		{
			ft_fprintf(2, "minishell: cd: No such file or directory\n");
			g_es(1, 0);
			return ;
		}
	}
	else
		ft_cd_handle_special_cases(var, old_pwd, NULL);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		pwd_upd_old(var, "PWD", cwd);
	else
	{
		perror("getcwd");
		g_es(1, 0);
		return ;
	}
	// g_es(0, 0);
}
