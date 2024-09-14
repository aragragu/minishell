/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:38:50 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/14 18:38:38 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_builtins(char *str)
{
	if (!str)
		return (-1);
	else if (!ft_strcmp(str, "echo") || !ft_strcmp(str, "cd") || \
			!ft_strcmp(str, "exit") || !ft_strcmp(str, "pwd") || \
			!ft_strcmp(str, "export") || !ft_strcmp(str, "unset"))
		return (1);
	return (0);
}

void	ft_builtins(t_var *var, char *str, t_cmd **cmd)
{
	(void)cmd;
	if (!ft_strcmp(str, "echo"))
		ft_echo(var);
	else if (!ft_strcmp(str, "exit"))
		ft_exit(var);
	else if (!ft_strcmp(str, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(str, "cd"))
		ft_cd(var);
	else if (!ft_strcmp(str, "unset"))
		ft_unset(var);
	else if (!ft_strcmp(str, "export"))
		ft_export(var, 0, 0);
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

int	count_env(t_env *envv)
{
	int	count;

	count = 0;
	while (envv)
	{
		count++;
		envv = envv->next;
	}
	return (count);
}

void	store_env(t_env *envv, char ***env)
{
	int	env_count;
	int	i;

	env_count = count_env(envv);
	*env = malloc((env_count + 1) * sizeof(char *));
	if (!*env)
	{
		perror("Failed to allocate memory for env");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (envv)
	{
		int len = ft_strlen(envv->key) + ft_strlen(envv->value) + 2;
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

	store_env(var->env, &envp);
	pid = fork();
	if (pid == 0)
	{
		
		// var->list->argc[0] = exec_path;
		if (execve(var->list->cmd, var->list->argc, envp) == -1)
		{
			perror("Execution failed");
			// free(exec_path);
		}
	}
	waitpid(pid, NULL, 0);
}

void	ft_exc(t_var *var)
{
	char	*exec_path;
	char	**envp;
	pid_t	pid;

	store_env(var->env, &envp);
	exec_path = excu_in_path(var->list->cmd, var);
	if (exec_path != NULL)
	{
		pid = fork();
		if (pid == 0)
		{
			
			var->list->argc[0] = exec_path;
			if (execve(exec_path, var->list->argc, envp) == -1)
			{
				perror("Execution failed");
				free(exec_path);
			}
		}
		waitpid(pid, NULL, 0);
	}
	else if (exec_path)
		return ;
}
