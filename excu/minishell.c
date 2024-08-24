/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:26:58 by ykasmi            #+#    #+#             */
/*   Updated: 2024/08/24 17:34:18 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (fd < 0)
		return ;
	if (!s)
		return ;
	while (s[i] != '\0')
	{
		write (fd, &s[i], 1);
		i++;
	}
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

void	ft_builtins(t_var *var, char *str)
{
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
	else
		puts("command not found");
}

int	main(int ac, char **av, char **env)
{
	t_var	var;
	char	cmd[] = "unset";
	char	*inp;
	char	*varr[100] = {"unset", "PWD",  NULL};
	(void)ac;
	(void)av;
	var.env = malloc(sizeof(t_env));
	var.list = malloc(sizeof(t_list));
	if (!var.env)
		return (1);
	var.list->cmd = cmd;
	var.list->arg = varr;
	init_env(&var.env, env);
	while (1)
	{
		inp = readline("\033[1;93mminishell $\033[0m ");
		if(!inp || !inp[0])
			continue;
		if (check_builtins(*varr))
			ft_builtins(&var, *varr);
		if (ft_strcmp(inp, "env") == 0)
			ft_env(&var.env);    
	}
}
