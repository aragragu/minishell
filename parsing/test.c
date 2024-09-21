
// void	execute_pipe(char *input, int num_cmds, t_var *var)
// {
// 	int		pipefd[2];
// 	int		prev_fd = STDIN_FILENO;
// 	char	*input_copy;
// 	char	**envp;
// 	char	*cmd;
// 	char	**args;
// 	char	*cmd_path;
// 	int		i = 0;
//     int in = 0;
//     int out = 1;
// 	pid_t	pid;

// 	input_copy = strdup(input);
// 	store_env(var->env, &envp);
// 	cmd = strtok(input_copy, "|");
// 	while (i < num_cmds)
// 	{
// 		if (i < num_cmds - 1)
// 			pipe(pipefd);
// 		pid = fork();
// 		if (pid < 0)
// 		{
// 			perror("fork failed");
// 			exit(EXIT_FAILURE);
// 		}
// 		else if (pid == 0)
// 		{
//             in = 0;
//             out = 1;
//             if (i < num_cmds - 1)
//                 close(pipefd[0]);
//             // if (y.tmp1->fd_in == -1)
//             // error_message("Minishell$: No such file or directory\n", 1);
//             if (out == 1 && i < num_cmds - 1)
//                 out = pipefd[1];
//             if (in == 0 && i != 0)
//                 in = prev_fd;
//             if (in != 0)
//                 dup2(in, STDIN_FILENO);
//             if (out != 1)
//                 dup2(out, STDOUT_FILENO);
//             if (i < num_cmds - 1)
//                 close(pipefd[1]);
//             if (in != 0)
//                 close(in);
//             if (out != 1)
//                 close(out);
// 			args = parse_command(cmd);
// 			cmd_path = excu_in_path(args[0], var);
// 			if (cmd_path)
// 			{
// 				execve(cmd_path, args, envp);
// 				free(cmd_path);
// 			}
// 			if (check_builtins(var->list->cmd))
//             {
// 				ft_builtins(var, var->list->cmd, &var->list);
//                 puts("ugade");
//             }
// 			else if (access(var->list->cmd, X_OK) == 0)
// 					ft_exc2(var);
// 			else
// 				fprintf(stderr, "minishell: %s: command not found\n", args[0]);
// 		}
//         if (i < num_cmds - 1)
//             close(pipefd[1]);
//         if (i != 0)
//             close(prev_fd);
//         prev_fd = pipefd[0];      
// 		cmd = strtok(NULL, "|");
// 		i++;
// 	}
	
    
    
// 	while (i > 0)
//     {
//         waitpid(-1, NULL, 0);
//         i--;
//     }
// 	// free(input_copy);
// }
