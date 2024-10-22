/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils8.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:48:48 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/23 00:11:57 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*char	*norm_excu_in_path(char *filename, t_var *var)
{
	struct stat	f_stat;

	if (!ft_strcmp(filename, "."))
	{
		ft_fprintf(2, "minishell: .: filename argument required\n");
		ft_fprintf(2, ".: usage: . filename [arguments]\n");
		exit(2);
	}
	stat(filename, &f_stat);
	if (filename[0] == '.' && filename[1] == '.' && filename[2] == '\0')
		error_function(var);
	else if (S_ISDIR(f_stat.st_mode))
	{
		ft_fprintf(2, "%s: is a directory\n", filename);
		exit(g_es(126, 0));
	}
	else if (!S_ISDIR(f_stat.st_mode) && ft_strchr(filename, '/'))
	{
		perror(filename);
		exit(g_es(127, 0));
	}
	else if (access(filename, X_OK) == 0)
		return (ft_strduppp(filename));
	return (NULL);
}*/

char	*norm_excu_in_path(char *filename)
{
	struct stat	f_stat;

	if (filename[0] == '/' || (filename[0] == '.' && filename[1] == '/'))
	{
		if (stat(filename, &f_stat) == 0)
		{
			if (S_ISDIR(f_stat.st_mode))
			{
				fprintf(stderr, "%s: is a directory\n", filename);
				exit(g_es(126, 0));
			}
		}
		if (access(filename, X_OK) == 0)
			return (ft_strduppp(filename));
		perror(filename);
		g_es(127, 0);
		exit(127);
	}
	else if (!ft_strcmp(filename, "."))
	{
		ft_fprintf(2, "minishell: .: filename argument required\n");
		ft_fprintf(2, ".: usage: . filename [arguments]\n");
		exit(2);
	}
	return (NULL);
}

char	*excu_in_path(char *filename, t_var *var)
{
	char	*path;
	char	*start;
	char	*result;

	path = ft_getenv(var->env, "PATH");
	if (!path)
	{
		if (var->path == NULL)
			return (NULL);
		path = var->path;
		if (!path)
			return (NULL);
	}
	var->flag = 0;
	start = ft_strduppp(path);
	result = search_in_path(start, filename, var);
	if (result)
	{
		free(var->ptr);
		return (result);
	}
	return (NULL);
}

t_redir	*check_redirection(t_redir *list)
{
	t_redir	*current;
	t_redir	*last;

	last = ft_lstlast_redi(list);
	current = list;
	if (!last)
		return (NULL);
	while (current)
	{
		if (current && current->type == HEREDOC \
			&& ft_strcmp(current->value, last->value))
			close(current->fd);
		current = current->next;
	}
	return (last);
}
