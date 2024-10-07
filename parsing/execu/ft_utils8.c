/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils8.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:48:48 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/07 14:28:43 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*norm_excu_in_path(char *filename, t_var *var)
{
	struct stat	f_stat;

	if (filename[0] == '/' || (filename[0] == '.' && filename[1] == '/'))
	{
		if (stat(filename, &f_stat) == 0)
		{
			if (S_ISDIR(f_stat.st_mode))
			{
				fprintf(stderr, "%s: is a directory\n", filename);
				var->exit_num = 126;
				exit(126);
			}
		}
		if (access(filename, X_OK) == 0)
			return (strdup(filename));
		perror(filename);
		var->exit_num = 127;
		exit(127);
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
	}
	var->flag = 0;
	start = ft_strduppp(path);
	result = search_in_path(start, filename, var);
	if (result)
	{
		free(var->ptr);
		return (result);
	}
	free(var->ptr);
	return (NULL);
}
