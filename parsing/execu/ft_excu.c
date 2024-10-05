/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_excu.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:53:26 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/05 18:19:09 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <libc.h>

char	*check_valid_path(char *filename, t_var *var)
{
	char	*exec_path;

	if (!filename || !*filename)
		return (NULL);
	exec_path = norm_excu_in_path(filename, var);
	if (exec_path)
		return (exec_path);
	exec_path = excu_in_path(filename, var);
	if (exec_path != NULL)
		return (free(exec_path), exec_path);
	return (NULL);
}

char	*ft_getenv(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key) && ft_strlen(key) == ft_strlen(tmp->key))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*build_path(char *dir, char *filename)
{
	size_t	len;
	char	*full_path;

	len = ft_strlen(dir) + ft_strlen(filename) + 2;
	full_path = malloc(len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, filename);
	return (full_path);
}

char	*excu_in_path(char *filename, t_var *var)
{
	char	*path;
	char	*start;
	char	*end;
	char	*full_path;

	path = ft_getenv(var->env, "PATH");
	if (!path)
	{
		if (var->path == NULL)
			return (NULL);
		path = var->path;
	}
	var->flag = 0;
	start = ft_strduppp(path);
	end = ft_strchrr(start, ':');
	while (end || (*start != '\0'))
	{
		if (!end)
			end = ft_strchrr(start, '\0');
		if (!end || !end[0])
			return (free(var->ptr), NULL);
		*end = '\0';
		full_path = build_path(start, filename);
		if (var->flag == 0)
		{
			var->flag = 1;
			var->ptr = start;
		}
		if (access(full_path, X_OK) == 0)
			return (free(var->ptr), full_path);
		free(full_path);
		start = end + 1;
		end = ft_strchrr(start, ':');
	}
	return (free(var->ptr), free(start), NULL);
}

int	calculate_cmd(t_var *var)
{
	int		num_cmd;
	t_cmd	*list;

	num_cmd = 0;
	list = var->list;
	while (list)
	{
		num_cmd++;
		list = list->next;
	}
	return (num_cmd);
}
