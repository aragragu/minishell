/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:21:52 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/26 16:39:46 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void handle_redirection(const char *filename, int type) {
//     int fd;
//     if (type == REDIR_OU)
// 	{ // '>'
//         fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (fd < 0) perror("open");
//         dup2(fd, STDOUT_FILENO);  // Redirect stdout
//     }
// 	else if (type == REDIR_IN)
// 	{ // '<'
//         fd = open(filename, O_RDONLY);
//         if (fd < 0) perror("open");
//         dup2(fd, STDIN_FILENO);  // Redirect stdin
//     }
// 	else if (type == APPEND)
// 	{ // '>>'
//         fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
//         if (fd < 0) perror("open");
//         dup2(fd, STDOUT_FILENO);  // Redirect stdout
//     }
// 	else if (type == HEREDOC)
//             write(pipe_fd[1], line, read);
//     close(fd);  // Close file descriptor after redirecting
// }

int	calculate_num_cmds(char *input)
{
	int		num_cmds;
	char	*input_copy;
	char	*token;

	num_cmds = 0;
	input_copy = strdup(input);
	if (!input_copy)
		return(0);
	token = strtok(input_copy, "|");
	while (token != NULL)
	{
		num_cmds++;
		token = strtok(NULL, "|");
	}
	return (free(input_copy), num_cmds);
}

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

void	handle_redirection2(t_var *var)
{
	t_redir *redir = var->list->redirection;
	int fd;

	while (redir)
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
		else if (redir->type == APPEND)
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
		redir = redir->next;
	}
}

void	execute_pipe(int num_cmds, t_var *var)
{
	int		pipefd[2];
	int		prev_fd = STDIN_FILENO;
	char	**envp;
	char	*cmd_path;
	int		i = 0;
	pid_t	pid;

	store_env(var->env, &envp);
	while (i < num_cmds)
	{
		if (i < num_cmds - 1)
			pipe(pipefd);
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			dup2(prev_fd, STDIN_FILENO);
			if (i < num_cmds - 1)
				dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);

			if (contains_red(var) == 0)
			{
				handle_redirection2(var);
				if (!var->list->argc || !var->list->argc[0])
					exit(0);
			}
			if (var->list->argc)
			{
				cmd_path = excu_in_path(var->list->argc[0], var);
				if (cmd_path)
				{
					execve(cmd_path, var->list->argc, envp);
					free(cmd_path);
				}
				else if (check_builtins(var->list->cmd))
				{
					ft_builtins(var, var->list->cmd, &var->list);
					exit(0);
				}
				else if (access(var->list->cmd, X_OK) == 0)
				{
					ft_exc2(var);
					exit(0);
				}
				else
				{
					fprintf(stderr, "minishell: %s: command not found\n", var->list->argc[0]);
					exit(0);
				}
			}
		}
        close(pipefd[1]);
        if (i != 0)
            close(prev_fd);
		prev_fd = pipefd[0];
		var->list = var->list->next;
		i++;
	}

	// Wait for all child processes to finish
	while (waitpid(-1, NULL, 0) > 0)
		;
}