/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:21:52 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/17 16:30:34 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char **parse_command(char *cmd)
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

void execute_pipe(char *input, int num_cmds, t_var *var)
{
	int		pipefd[2];
	int		prev_fd = 0;
	char	*input_copy;
	char	**envp;
	char	*cmd;
	char	**args;
	char	*cmd_path;
	int		i = 0;

	input_copy = strdup(input);
	store_env(var->env, &envp);
	cmd = strtok(input_copy, "|");
	while (i < num_cmds)
	{
		pipe(pipefd);
		if (fork() == 0)
		{
			dup2(prev_fd, 0);
			if (i < num_cmds - 1)
				dup2(pipefd[1], 1);
			close(pipefd[0]);
			close(pipefd[1]);
			args = parse_command(cmd);
			cmd_path = excu_in_path(args[0], var);
			if (cmd_path)
			{
				if (execve(cmd_path, args, envp) == -1)
					perror("execve failed");
				free(cmd_path);
			}
		}
		close(pipefd[1]);
		prev_fd = pipefd[0];
		cmd = strtok(NULL, "|");
		i++;
	}
	close(prev_fd);
	while (wait(NULL) > 0);
	free(input_copy);
}

int	contains_pipe(char *input)
{
    return (ft_strchr(input, '|') != NULL);
}