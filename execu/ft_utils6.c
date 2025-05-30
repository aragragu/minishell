/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:21:52 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/23 13:26:12 by ykasmi           ###   ########.fr       */
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
			exit(g_es(0, 1));
		}
		else
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			execve(cmd_path, var->list->argc, envp);
			free(cmd_path);
			error_function(var);
		}
	}
}

void	norm_excu_pipe2(int prev_fd, int i, int num_cmds, int pipefd[2])
{
	(void)prev_fd;
	dup2(prev_fd, STDIN_FILENO);
	if (i < num_cmds - 1)
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
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

void	norm_pipe(t_var *var, t_cmd *list, int flag, struct termios *term)
{
	if (flag == 0)
	{
		save_std_in_out(list, 0);
		store_env(var->env, &var->envp, 0, 0);
	}
	else
	{
		ft_free(var->envp);
		save_std_in_out(list, 1);
		waitpid_func(var, term);
	}
}

void	execute_pipe(int num_cmds, t_var *var, int i, int prev_fd)
{
	int				pipefd[2];
	t_redir			*last_herdoc;

	tcgetattr(STDIN_FILENO, &var->term);
	var->list2 = var->list;
	norm_pipe(var, var->list2, 0, &var->term);
	while (++i < num_cmds)
	{
		last_herdoc = check_redirection(var->list->redirection);
		(i < num_cmds - 1) && (pipe(pipefd), 0);
		signal(SIGQUIT, signal_hand_sig_qui);
		var->pid[i] = fork();
		if (var->pid[i] == -1)
			(1) && (ft_free(var->envp), error_fork(), 0);
		if (var->pid[i] == 0)
		{
			norm_excu_pipe2(prev_fd, i, num_cmds, pipefd);
			(1) && (norm_excu_pipe3(&var), norm_excu_pipe(var, var->envp), 0);
		}
		close(pipefd[1]);
		(i != 0) && (close(prev_fd), 0);
		(last_herdoc) && (close(last_herdoc->fd), 0);
		(prev_fd = pipefd[0]) && (var->list = var->list->next, 0);
	}
	norm_pipe(var, var->list2, 1, &var->term);
}
