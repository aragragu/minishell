/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:31:11 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/09 15:51:50 by ykasmi           ###   ########.fr       */
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

void	error_fun_cd(t_var *var, char *home, int flag)
{
	if (flag == 0)
	{
		perror(var->list->argc[1]);
		var->exit_num = 1;
	}
	else if (flag == 1)
	{
		ft_fprintf(2, "minishell: cd: HOME not set\n");
		var->exit_num = 1;
	}
	else if (flag == 2)
	{
		ft_fprintf(2, "cd:%s:No such file or directory\n", home);
		var->exit_num = 1;
	}
}

void	ft_cd_handle_special_cases(t_var *var, char *old_pwd, char *home)
{
	if (var->list->argc[1] == NULL || !ft_strcmp(var->list->argc[1], "~"))
	{
		if (var->list->argc[1] == NULL)
			home = ft_getenv(var->env, "HOME");
		else
			home = getenv("HOME");
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
	pwd_upd_old(&var->env, "OLDPWD", old_pwd);
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
			var->exit_num = 1;
			return ;
		}
	}
	else
		ft_cd_handle_special_cases(var, old_pwd, NULL);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		pwd_upd_old(&var->env, "PWD", cwd);
	else
	{
		perror("getcwd");
		var->exit_num = 1;
		return ;
	}
	var->exit_num = 0;
}
