/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:21:52 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/28 18:26:23 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	contains_red(t_var *var)
{
	t_redir *redir = var->list->redirection;

	if (redir && redir->type == REDIR_OUT)
		return (0);
	else if (redir && redir->type == REDIR_IN)
		return (0);
	else if (redir && redir->type == APPEND)
		return (0);
	else if (redir && redir->type == HEREDOC)
		return (0);
	return (1);
}

void	red_out_in(t_redir *redir, int fd)
{
	if (redir->type == REDIR_OUT)
	{
		fd = open(redir->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open failed");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_IN)
	{
		fd = open(redir->value, O_RDONLY);
		if (fd < 0)
		{
			perror("open failed");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	red_herd_appen(t_redir *redir, int fd)
{
	if (redir->type == APPEND)
	{
		fd = open(redir->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("open failed");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type == HEREDOC)
	{
		if (redir->fd < 0)
		{
			perror("open failed");
			exit(EXIT_FAILURE);
		}
		dup2(redir->fd, STDIN_FILENO);
		close(redir->fd);
	}
}

void	handle_redirection2(t_var *var)
{
	t_redir *redir = var->list->redirection;
	int fd = 0;

	while (redir)
	{
		red_out_in(redir, fd);
		red_herd_appen(redir, fd);
		redir = redir->next;
	}
}

void	error_function(t_var *var)
{
	fprintf(stderr, "minishell: %s: command not found\n", var->list->argc[0]);
	exit(0);
}

void	norm_excu_pipe(t_var *var, char **envp)
{
	char	*cmd_path;

	if (var->list->argc)
	{
		cmd_path = excu_in_path(var->list->argc[0], var);
		if (check_builtins(var->list->cmd))
		{
			ft_builtins(var, var->list->cmd, &var->list);
			exit(0);
		}
		else if (cmd_path)
		{
			execve(cmd_path, var->list->argc, envp);
			free(cmd_path);
		}
		else if (access(var->list->cmd, X_OK) == 0)
		{
			ft_exc2(var);
			exit(0);
		}
		else
			error_function(var);
	}
}

void	error_fork(pid_t pid)
{
	if (pid < 0)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
}

void	norm_excu_pipe2(int prev_fd, int i, int num_cmds, int pipefd[2])
{
	dup2(prev_fd, STDIN_FILENO);
	if (i < num_cmds - 1)
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	norm_excu_pipe3(t_var **var)
{
	if (contains_red(*var) == 0)
	{
		handle_redirection2(*var);
		if (!(*var)->list->argc || !(*var)->list->argc[0])
			exit(0);
	}
}

void	waitpid_func(void)
{
	while (waitpid(-1, NULL, 0) > 0)
		;
}

void	execute_pipe(int num_cmds, t_var *var, int i)
{
	int		pipefd[2];
	int		prev_fd = STDIN_FILENO;
	char	**envp;
	pid_t	pid;

	store_env(var->env, &envp);
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
	waitpid_func();
}
