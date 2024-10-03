/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils8.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:48:48 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/03 18:15:53 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*excu_in_path2(char *filename, t_var *var)
{
	char	*start;
	char	*end;
	char	*full_path;

	var->flag = 0;
	start = ft_strduppp(var->path);
	end = ft_strchrr(start, ':');
	while (end || (*start != '\0'))
	{
		if (!end)
			end = ft_strchrr(start, '\0');
		if (!end || !end[0])
			return (free(var->ptr),  NULL);
		*end = '\0';
		full_path = build_path(start, filename);
		if (var->flag == 0)
		{
			var->flag = 1;
			var->ptr = start;
		}
		if (full_path && access(full_path, X_OK) == 0)
			return (free(var->ptr), full_path);
		free(full_path);
		start = end + 1;
		end = ft_strchrr(start, ':');
	}
	return (free(var->ptr), free(start), NULL);
}

void	ft_exc3(t_var *var)
{
	char	**envp;
	char	*cmd_path;
	pid_t	pid;

	cmd_path = excu_in_path2(var->list->argc[0], var);
	store_env(var->env, &envp, 0, 0);
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, var->list->argc, envp) == -1)
			perror(var->list->argc[0]);
	}
	ft_free(envp);
	waitpid(pid, NULL, 0);
}