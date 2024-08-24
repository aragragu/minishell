/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 16:54:07 by ykasmi            #+#    #+#             */
/*   Updated: 2024/08/24 17:33:04 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(t_var *var)
{
	char	cwd[PATH_MAX];
	char	old_pwd[PATH_MAX];
	char	*home;

	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	if (var->list->arg[1] == NULL)
	{
		home = getenv("HOME");
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
		if (chdir(var->list->arg[1]) != 0)
		{
			printf("cd: %s: No such file or directory\n", var->list->arg[1]);
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
