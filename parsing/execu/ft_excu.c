/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_excu.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:53:26 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/30 20:57:37 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_valid_path(char *filename, t_var *var)
{
	char	*exec_path;

	if (!filename || !*filename)
		return (-1);
	exec_path = excu_in_path(filename, var);
	if (exec_path != NULL)
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
	char	*start;
	char	*end;
	char	*full_path;

	path = ft_getenv(var->env, "PATH");
	start = ft_strduppp(path);
	if (!path)
		return (NULL);
	end = ft_strchr(start, ':');
	while (end || (*start != '\0'))
	{
		if (!end)
			end = ft_strchr(start, '\0');
		if (!end || !end[0])
			return (NULL);
		*end = '\0';
		full_path = build_path(start, filename);
		if (full_path && access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		start = end + 1;
		end = ft_strchr(start, ':');
	}
	return (free(start), NULL);
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


// char	*excu_in_path(char *filename, t_var *var)
// {
// 	char	*path;
// 	// char	*start;
// 	// char	*end;
// 	char	*full_path;
// 	char 	**tab;
// 	int		i;

// 	i = 0;
// 	(void)filename;
// 	path = ft_getenv(var->env, "PATH");
// 	if (!path)
// 		return (NULL);
// 	tab = ft_split2(path, ':');
// 	if (!tab)
// 		return(NULL);
// 	while (tab[i])
// 	{
// 		full_path = ft_strjoinnn(tab[i], var->list->cmd);
// 		if (access(full_path, X_OK))
// 			return (full_path);
// 	}
// 	return ("");
// 	// start = ft_strduppp(path);
// 	// end = ft_strchr(start, ':');
// 	// while (end || (*start != '\0'))
// 	// {
// 	// 	if (!end)
// 	// 		end = ft_strchr(start, '\0');
// 	// 	if (!end || !end[0])
// 	// 		return (NULL);
// 	// 	*end = '\0';
// 	// 	full_path = build_path(start, filename);
// 	// 	if (full_path && access(full_path, X_OK) == 0)
// 	// 		return (full_path);
// 	// 	free(full_path);
// 	// 	start = end + 1;
// 	// 	end = ft_strchr(end, ':');
// 	// 	printf ("%s\n", start);
// 	// }
// 	// return (free(start), NULL);
// }