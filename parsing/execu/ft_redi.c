/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:51:38 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/20 16:31:59 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	norm_excu_pipe3(t_var **var)
{
	if (contains_red(*var) == 0)
	{
		handle_redirection2(*var);
		if (!(*var)->list->argc || !(*var)->list->argc[0])
			exit(0);
	}
}

int	contains_red(t_var *var)
{
	t_redir	*redir;

	redir = var->list->redirection;
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
			exit(1);
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
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	red_herd_appen(t_redir *redir, int fd, t_var *var)
{
	(void)var;
	if (redir->type == APPEND)
	{
		fd = open(redir->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("open failed");
			exit(errno);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type == HEREDOC)
	{
		if (redir->fd < 0)
		{
			perror("open failed");
			exit(errno);
		}
		dup2(redir->fd, STDIN_FILENO);
	}
}

void	handle_redirection2(t_var *var)
{
	t_redir	*redir;
	int		fd;

	fd = 0;
	redir = var->list->redirection;
	while (redir)
	{
		if (redir->value == NULL)
		{
			ft_fprintf(2, "minishell: ambiguous redirect\n");
			g_es(1, 0);
			exit(1);
		}
		red_out_in(redir, fd);
		red_herd_appen(redir, fd, var);
		redir = redir->next;
	}
	g_es(0, 0);
}
