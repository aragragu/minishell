/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:37:42 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/21 18:49:15 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	open_herdoc(t_elem **list, t_env **env, t_garbage **garbage)
{
	static int	i;
	int			flag;
	t_herdoc	*data;

	data = NULL;
	initiaize_herdoc(&data, &i, garbage, list);
	while (1)
	{
		flag = herdoc_loop(list, data, garbage, env);
		if (flag == 1)
			break ;
		if (flag == 2)
			continue ;
	}
	if (!isatty(STDIN_FILENO))
	{
		sigint_herdoc();
		(*list)->fd_here = -2;
		close(data->cfd);
		return ;
	}
	(*list)->content = data->file_name;
	(*list)->type = HEREDOC;
	(*list)->fd = data->cfd;
	(*list)->fd_here = 0;
}

int	fd_here_checker(t_elem *list)
{
	while (list)
	{
		if (list->fd_here == -2)
			return (1);
		list = list->next;
	}
	return (0);
}

void	initiaize_herdoc(t_herdoc **data, int *i, t_garbage **garbage,
t_elem **list)
{
	*data = ft_lstnew3(list, ft_strdup("", garbage), garbage);
	if (!*data)
		return ;
	(*data)->file_name = (ft_strjoin(ft_strdup("tmp_", garbage),
				ft_itoa(++(*i), garbage), garbage));
	(*data)->fd = 0;
	(*data)->cfd = 0;
	signal(SIGINT, s_handler);
	(*data)->line = NULL;
}

int	herdoc_loop(t_elem **list, t_herdoc *data, t_garbage **garbage, t_env **env)
{
	int		i;
	int		j;

	i = check_herdoc_line(list, data, garbage);
	if (i == 1)
		return (1);
	if (i == 2)
		return (2);
	j = write_herdoc_line(list, data, garbage, env);
	if (j == 1)
		return (1);
	if (j == 2)
		return (2);
	return (0);
}

void	open_herdoc_file(t_herdoc *data)
{
	data->fd = open(data->file_name, O_CREAT | O_RDWR, 0644);
	data->cfd = open(data->file_name, O_RDONLY);
	if (data->fd == -1 || data->cfd == -1)
	{
		perror("Error opening file");
		return ;
	}
	unlink(data->file_name);
	write(data->fd, data->buffer, ft_strlen(data->buffer));
	close (data->fd);
}
