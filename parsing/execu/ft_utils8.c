/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils8.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:48:48 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/11 00:22:58 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	nb_counter(long nb)
{
	int	count;

	count = 0;
	if (nb < 0)
	{
		count++;
		nb = nb * (-1);
	}
	if (nb == 0)
	{
		count++;
	}
	while (nb != 0)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

static char	*a_lloc(int len)
{
	char	*temp;

	temp = malloc (len + 1);
	if (temp == 0)
		return (NULL);
	temp[0] = '0';
	return (temp);
}

char	*ft_itoa1(int n)
{
	long	nb;
	int		i;
	int		len;
	char	*res;

	nb = n;
	len = nb_counter(nb);
	res = a_lloc(len);
	if (res == NULL)
		return (NULL);
	if (nb < 0)
		nb = nb * (-1);
	i = len - 1;
	while (nb != 0)
	{
		res[i] = (nb % 10) + '0';
		nb = nb / 10;
		i--;
	}
	if (n < 0)
		res[0] = '-';
	res[len] = '\0';
	return (res);
}

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
				g_es(126, 0);
				exit(126);
			}
		}
		if (access(filename, X_OK) == 0)
			return (ft_strduppp(filename));
		perror(filename);
		g_es(127, 0);
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
