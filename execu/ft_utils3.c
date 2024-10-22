/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:38:50 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/22 16:40:25 by ykasmi           ###   ########.fr       */
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

void	store_env(t_env *envv, char ***env, int i, int len)
{
	int	env_count;

	env_count = count_env(envv);
	*env = malloc((env_count + 1) * sizeof(char *));
	if (!*env)
	{
		perror("Failed to allocate memory for env");
		exit(1);
	}
	while (envv)
	{
		len = ft_strlen(envv->key) + ft_strlen(envv->value) + 2;
		(*env)[i] = malloc(len * sizeof(char));
		if (!(*env)[i])
		{
			perror("Failed to allocate memory for env variable");
			exit(1);
		}
		ft_strcpy((*env)[i], envv->key);
		ft_strcat((*env)[i], "=");
		ft_strcat((*env)[i], envv->value);
		envv = envv->next;
		i++;
	}
	(*env)[i] = NULL;
}

void	update_exit_status(int status)
{
	int	stat;

	stat = 0;
	if (WIFEXITED(status))
		stat = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		stat = 128 + WTERMSIG(status);
	g_es(stat, 0);
}

void	function_handle_sigqu(struct termios term)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	write(1, "Quit: 3\n", 8);
}

void	ft_exc(t_var *var)
{
	char			*exec_path;
	char			**envp;
	int				exit_stat;
	pid_t			pid;
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	store_env(var->env, &envp, 0, 0);
	pid = fork();
	if (pid == -1)
		return (ft_free(envp), error_fork());
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		(!var->list->argc[0][0]) && (error_function(var), 0);
		exec_path = check_valid_path(var->list->cmd, var);
		execve(exec_path, var->list->argc, envp);
		(1) && (free(exec_path), error_function(var), 0);
	}
	ft_free(envp);
	waitpid(pid, &exit_stat, 0);
	if (WIFSIGNALED(exit_stat) && WTERMSIG(exit_stat) == SIGQUIT)
		function_handle_sigqu(term);
	update_exit_status(exit_stat);
}
