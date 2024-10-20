/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 11:29:24 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 16:34:21 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	if (list && ((*list)->type == REDIR_OUT || (*list)->type == REDIR_IN
			|| (*list)->type == APPEND))
		starting_red(list);
	while (current)
	{
		if (current->next && current->next->type == REDIR_OUT)
			redirection_out_list(&current);
		else if (current->next && current->next->type == REDIR_IN)
			redirection_in_list(&current);
		else if (current->next && current->next->type == APPEND)
			append_list(&current);
		current = current->next;
	}
}

int	check_fd_her(t_elem **elem)
{
	t_elem	*list;

	list = *elem;
	while (list)
	{
		if (list->type == HEREDOC && list->fd_here == -2)
			return (1);
		list = list->next;
	}
	return (0);
}

void	starting_red(t_elem **list)
{
	t_elem	*current;

	current = *list;
	if (current && current->type == REDIR_IN)
		s_redir_in(list);
	else if (current && current->type == REDIR_OUT)
		s_redir_out(list);
	else if (current && current->type == APPEND)
		s_append(list);
}

void	s_redir_in(t_elem **list)
{
	t_elem	*current;
	t_elem	*holder;

	current = *list;
	if (current->next && current->next->type == S_PACE)
	{
		holder = current->next;
		if (holder->next && holder->next->type < S_PACE)
		{
			*list = holder->next;
			holder->next->type = REDIR_IN;
			return ;
		}
	}
	else if (current->next && current->next->type < S_PACE)
	{
		*list = current->next;
		current->next->type = REDIR_IN;
		return ;
	}
}

void	s_redir_out(t_elem **list)
{
	t_elem	*current;
	t_elem	*holder;

	current = *list;
	if (current->next && current->next->type == S_PACE)
	{
		holder = current->next;
		if (holder->next && holder->next->type < S_PACE)
		{
			*list = holder->next;
			holder->next->type = REDIR_OUT;
			return ;
		}
	}
	else if (current->next && current->next->type < S_PACE)
	{
		*list = current->next;
		current->next->type = REDIR_OUT;
		return ;
	}
}

void	s_append(t_elem **list)
{
	t_elem	*current;
	t_elem	*holder;

	current = *list;
	if (current->next && current->next->type == S_PACE)
	{
		holder = current->next;
		if (holder->next && holder->next->type < S_PACE)
		{
			*list = holder->next;
			holder->next->type = APPEND;
			return ;
		}
	}
	else if (current->next && current->next->type < S_PACE)
	{
		*list = current->next;
		current->next->type = APPEND;
		return ;
	}
}

void	redirection_out_list(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	if (current && current->next)
	{
		if (current->next->type == REDIR_OUT
			&& !ft_strcmp(current->next->content, ">"))
			redirection_out_list2(list);
	}
}

void	redirection_out_list2(t_elem **list)
{
	t_elem	*current;
	t_elem	*red_out;

	current = *list;
	red_out = current->next;
	if (red_out && red_out->next)
	{
		if (red_out->next && red_out->next->type < S_PACE)
		{
			red_out->next->type = REDIR_OUT;
			current->next = red_out->next;
			return ;
		}
		else if (red_out->next->next && red_out->next->type == S_PACE)
		{
			red_out = red_out->next->next;
			if (red_out->type < S_PACE)
			{
				red_out->type = REDIR_OUT;
				current->next = red_out;
				return ;
			}
		}
	}
}

void	redirection_in_list(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	if (current && current->next)
	{
		if (current->next->type == REDIR_IN
			&& !ft_strcmp(current->next->content, "<"))
			redirection_in_list2(list);
	}
}

void	redirection_in_list2(t_elem **list)
{
	t_elem	*current;
	t_elem	*red_out;

	current = *list;
	red_out = current->next;
	if (red_out && red_out->next)
	{
		if (red_out->next && red_out->next->type < S_PACE)
		{
			red_out->next->type = REDIR_IN;
			current->next = red_out->next;
			return ;
		}
		else if (red_out->next->next && red_out->next->type == S_PACE)
		{
			red_out = red_out->next->next;
			if (red_out->type < S_PACE)
			{
				red_out->type = REDIR_IN;
				current->next = red_out;
				return ;
			}
		}
	}
}

void	s_handler(int sig)
{
	(void)sig;
	if (sig == SIGINT)
	{
		g_sig = 2;
		close(0);
	}
}

void	sigint_herdoc(void)
{
	int	std_in;

	std_in = open(ttyname(STDERR_FILENO), O_RDONLY, 0644);
	if (std_in < 0)
		perror("open");
}

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
void	initiaize_herdoc(t_herdoc **data, int *i, t_garbage **garbage, t_elem **list)
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

int		herdoc_loop(t_elem **list, t_herdoc *data, t_garbage **garbage, t_env **env)
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

int	check_herdoc_line(t_elem **list, t_herdoc *data, t_garbage **garbage)
{
	data->line = readline(">");
	if (!data->line)
	{
		open_herdoc_file(data);
		return (1);
	}
	if (!ft_strcmp((*list)->content, data->line))
	{
		open_herdoc_file(data);
		return (1);
	}
	if (!*data->line)
	{
		free(data->line);
		return (2);
	}
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(data->line));
	return (0);
}

int	write_herdoc_line(t_elem **list, t_herdoc *data, t_garbage **garbage, t_env **env)
{
	if (!(*list)->ignore[1])
		expand_d_qouts_2(env, &data->line, garbage);
	if (!data->line)
		return (2);
	data->tmp = ft_strjoin(data->line, "\n", garbage);
	if (!data->tmp)
		return (1);
	data->tmp = ft_strjoin(data->buffer, data->tmp, garbage);
	if (!data->tmp)
		return (1);
	data->buffer = data->tmp;
	return (0);
}

void	append_list(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	if (current && current->next)
	{
		if (current->next->type == APPEND
			&& !ft_strcmp(current->next->content, ">>"))
			append_list2(list);
	}
}

void	append_list2(t_elem **list)
{
	t_elem	*current;
	t_elem	*red_out;

	current = *list;
	red_out = current->next;
	if (red_out && red_out->next)
	{
		if (red_out->next && red_out->next->type < S_PACE)
		{
			red_out->next->type = APPEND;
			current->next = red_out->next;
			return ;
		}
		else if (red_out->next->next && red_out->next->type == S_PACE)
		{
			red_out = red_out->next->next;
			if (red_out->type < S_PACE)
			{
				red_out->type = APPEND;
				current->next = red_out;
				return ;
			}
		}
	}
}

void	expand_herdoc(char **str, t_env **env, t_garbage **garbage)
{
	char	*gtr;
	t_env	*list;

	gtr = *str;
	if (*gtr == '$')
	{
		list = *env;
		if (*(gtr + 1) >= '0' && *(gtr + 1) <= '9')
			*str = ft_strdup("", garbage);
		else
		{
			while (list)
			{
				if (!ft_strcmp(list->key, gtr + 1))
				{
					*str = ft_strdup(list->value, garbage);
					return ;
				}
				list = list->next;
			}
			*str = ft_strdup("", garbage);
		}
	}
}
