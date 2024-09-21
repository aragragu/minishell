/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:21:52 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/21 18:43:29 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int handle_input_redirection(char **args)
{
    int i = 0;
    int fd;

    while (args[i])
    {
        puts("1");
        if (ft_strcmp(args[i], "<") == 0)
        {
            fd = open(args[i + 1], O_RDONLY);
            if (fd < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
            return 1;
        }
        i++;
    }
    return 0;
}

int handle_output_redirection(char **args)
{
    int i = 0;
    int fd;

    while (args[i])
    {
        puts("2");
        if (strcmp(args[i], ">") == 0)
        {
            fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
            return 1;
        }
        i++;
    }
    return 0;
}

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

char	**parse_command(char *cmd)
{
	int		size;  
	char	**args;
	char	*arg;
	int		i;

	i = 0;
	size = ft_strlen(cmd) + 1;
	args = malloc(size * sizeof(char *));
	arg = strtok(cmd, " ");
	while (arg != NULL) {
		args[i++] = arg;
		arg = strtok(NULL, " ");
	}
	args[i] = NULL;
	return (args);
}

void	execute_pipe(char *input, int num_cmds, t_var *var)
{
	int		pipefd[2];
	int		prev_fd = STDIN_FILENO;
	char	*input_copy;
	char	**envp;
	char	*cmd;
	char	**args;
	char	*cmd_path;
	int		i = 0;
	pid_t	pid;

	input_copy = strdup(input);
	store_env(var->env, &envp);
	cmd = strtok(input_copy, "|");
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
			args = parse_command(cmd);
            // handle_input_redirection(args);
            // handle_output_redirection(args);
			cmd_path = excu_in_path(args[0], var);
			if (cmd_path)
			{
				execve(cmd_path, args, envp);
				free(cmd_path);
			}
			if (check_builtins(var->list->cmd))
				ft_builtins(var, var->list->cmd, &var->list);
			else if (access(var->list->cmd, X_OK) == 0)
					ft_exc2(var);
			else
            {
				fprintf(stderr, "minishell: %s: command not found\n", args[0]);
                exit(0);
            }
		}
        close(pipefd[1]);
        if (i != 0)
            close(prev_fd);
		prev_fd = pipefd[0];
		cmd = strtok(NULL, "|");
		i++;
	}
	while (waitpid(-1, NULL, 0) > 0)
        ;
}

int	contains_pipe(char *input)
{
    return (ft_strchr(input, '|') != NULL);
}
