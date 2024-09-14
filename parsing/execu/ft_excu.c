/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_excu.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:53:26 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/14 15:06:25 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_valid_path(char *filename, t_var *var)
{
	char	*exec_path;

	if (!filename || !*filename)
		return (-1);
	exec_path = excu_in_path(filename, var);
	if (exec_path != NULL && ft_strcmp(exec_path, "/bin/"))
	{
		free(exec_path);
		return (1);
	}
	return (0);
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
	char	*c_path;
	char	*start;
	char	*end;
	char	*full_path;

	path = ft_getenv(var->env, "PATH");
	c_path = ft_strduppp(path);
	if (!path || !c_path)
		return (NULL);
	start = c_path;
	while ((end = ft_strchr(start, ':')) || (*start != '\0' && (end = ft_strchr(start, '\0'))))
	{
		if (!end || !end[0])
			return (NULL);
		*end = '\0';
		full_path = build_path(start, filename);
		if (full_path && access(full_path, X_OK) == 0)
			return (free(c_path), full_path);
		free(full_path);
		start = end + 1;
	}
	return (free(c_path), NULL);
}
