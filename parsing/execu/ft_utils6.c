/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:21:52 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/08 18:38:09 by ykasmi           ###   ########.fr       */
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
			exit(var->exit_num);
		}
		else
		{
			execve(cmd_path, var->list->argc, envp);
			error_function(var);
			free(cmd_path);
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

void	save_std_in_out(t_cmd *list, int flag)
{
	if (flag == 0)
	{
		list->fd[0] = dup(0);
		list->fd[1] = dup(1);
	}
	else
	{
		dup2(list->fd[0], STDIN_FILENO);
		dup2(list->fd[1], STDOUT_FILENO);
		close(list->fd[0]);
		close(list->fd[1]);
	}
}

void	execute_pipe(int num_cmds, t_var *var, int i, int prev_fd)
{
	int		pipefd[2];
	char	**envp;
	t_cmd	*list;
	pid_t	pid;

	list = var->list;
	save_std_in_out(list, 0);
	store_env(var->env, &envp, 0, 0);
	while (++i < num_cmds)
	{
		if (i < num_cmds - 1)
			pipe(pipefd);
		pid = fork();
		if (pid == -1)
			return (error_fork(pid));
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
	save_std_in_out(list, 1);
	ft_free(envp);
	waitpid_func(var);
}
