/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:21:52 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/05 18:09:40 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	norm_excu_pipe(t_var *var, char **envp)
{
	char	*cmd_path;

	if (var->list->argc)
	{
		cmd_path = check_valid_path(var->list->cmd, var);
		if (check_builtins(var->list->cmd))
		{
			ft_builtins(var, var->list->cmd, &var->list);
			var->exit_num = 0;
			exit(0);
		}
		else
		{
			execve(cmd_path, var->list->argc, envp);
			error_function(var);
			free(cmd_path);
			exit (127);
		}
	}
}

void	norm_excu_pipe2(int prev_fd, int i, int num_cmds, int pipefd[2])
{
	dup2(prev_fd, STDIN_FILENO);
	if (i < num_cmds - 1)
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
}

void	execute_pipe(int num_cmds, t_var *var, int i, int prev_fd)
{
	int		pipefd[2];
	char	**envp;
	pid_t	pid;

	store_env(var->env, &envp, 0, 0);
	while (++i < num_cmds)
	{
		if (i < num_cmds - 1)
			pipe(pipefd);
		pid = fork();
		error_fork(pid);
		if (pid == 0)
		{
			norm_excu_pipe2(prev_fd, i, num_cmds, pipefd);
			norm_excu_pipe3(&var);
			norm_excu_pipe(var, envp);
		}
		close(pipefd[1]);
		if (i != 0)
			close(prev_fd);
		prev_fd = pipefd[0];
		var->list = var->list->next;
	}
	ft_free(envp);
	waitpid_func();
}
