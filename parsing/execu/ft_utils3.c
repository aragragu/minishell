/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:38:50 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/30 21:22:30 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

void	env_key_error(char **cmd, t_env **env, int i, char *msg)
{
	(void)env;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(cmd[i], 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void	store_env(t_env *envv, char ***env, int i, int len)
{
	int	env_count;

	env_count = count_env(envv);
	*env = malloc((env_count + 1) * sizeof(char *));
	if (!*env)
	{
		perror("Failed to allocate memory for env");
		exit(EXIT_FAILURE);
	}
	while (envv)
	{
		len = ft_strlen(envv->key) + ft_strlen(envv->value) + 2;
		(*env)[i] = malloc(len * sizeof(char));
		if (!(*env)[i])
		{
			perror("Failed to allocate memory for env variable");
			exit(EXIT_FAILURE);
		}
		ft_strcpy((*env)[i], envv->key);
		ft_strcat((*env)[i], "=");
		ft_strcat((*env)[i], envv->value);
		envv = envv->next;
		i++;
	}
	(*env)[i] = NULL;
}

void	ft_exc2(t_var *var)
{
	char	**envp;
	pid_t	pid;

	store_env(var->env, &envp, 0, 0);
	pid = fork();
	if (pid == 0)
	{
		if (execve(var->list->cmd, var->list->argc, envp) == -1)
			perror(var->list->argc[0]);
	}
	waitpid(pid, NULL, 0);
}

void	ft_exc(t_var *var)
{
	char	*exec_path;
	char	**envp;
	pid_t	pid;

	store_env(var->env, &envp, 0, 0);
	pid = fork();
	if (pid == 0)
	{
		if (!var->list->argc[0][0])
		{
			ft_fprintf(2, "%s: command not found\n", var->list->argc[0]);
			exit(0);
		}
		exec_path = excu_in_path(var->list->argc[0], var);
		if (exec_path)
		{
			if (execve(exec_path, var->list->argc, envp) == -1)
				perror(var->list->argc[0]);
			free(exec_path);
		}
		else
			ft_fprintf(2, "%s: command not found\n", var->list->argc[0]);
	}
	waitpid(pid, NULL, 0);
}
