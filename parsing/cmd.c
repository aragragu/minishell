/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:30:40 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/18 17:38:42 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	import_data(t_cmd **cmd, t_elem **list, t_garbage **garbage)
{
	t_elem	*current;
	int		pipe;

	if (!*list)
		return ;
	current = *list;
	pipe = 0;
	rename_token(list);
	while (current)
	{
		if (current->type == PIPE)
			pipe++;
		current = current->next;
	}
	if (!pipe)
		fill_cmd1(cmd, list, garbage);
	else
		fill_cmd2(cmd, list, garbage);
}

int	count_word(t_elem **list)
{
	int		i;
	t_elem	*current;

	current = *list;
	i = 0;
	while (current)
	{
		if (current->content && current->type == WORD)
			i++;
		current = current->next;
	}
	return (i);
}

int	count_word2(t_elem **list)
{
	int		i;
	t_elem	*current;

	current = *list;
	i = 0;
	while (current)
	{
		if (current->type == PIPE)
			break ;
		if (current->content && current->type == WORD)
			i++;
		current = current->next;
	}
	return (i);
}

char	**allocate_str(int i, char **str, t_cmd *command, t_garbage **grb)
{
	str = (char **)malloc(sizeof(char *) * (i + 1));
	if (!str)
		return (NULL);
	ft_lstadd_back_garbage(grb, ft_lstnew_garbage(str));
	str[i] = NULL;
	command->argc = str;
	return (str);
}

void	fill_command(t_elem *current, char **str, t_cmd *command, t_garbage **grb)
{
	int	j;
	int	argc;

	(1) && (j = 0, argc = 0, 0);
	while (current)
	{
		if (current->type >= REDIR_IN && current->type <= APPEND)
			ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi
				(current->content, current->type, current->fd, grb));
		else if (current->type == WORD)
		{
			if (current && current->content && j == 0 && str[argc])
			{
				command->cmd = current->content;
				str[argc++] = current->content;
				j++;
			}
			else if (current && current->content)
			{
				str[argc++] = current->content;
			}
		}
		current = current->next;
	}
	str[argc] = NULL;
}

void	fill_command2(t_elem *current, char **str, t_cmd *command,
		t_garbage **grb)
{
	while (current)
	{
		if (current->type == WORD)
		{
			command->cmd = current->content;
			str = (char **)malloc(sizeof(char *) * (2));
			if (!str)
				return ;
			ft_lstadd_back_garbage(grb, ft_lstnew_garbage(str));
			command->argc = str;
			str[0] = current->content;
			str[1] = NULL;
		}
		if (current->type >= REDIR_IN && current->type <= APPEND)
			ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi
				(current->content, current->type, current->fd, grb));
		current = current->next;
	}
}

void	fill_cmd1(t_cmd **cmd, t_elem **list, t_garbage **garbage)
{
	int		i;
	t_elem	*current;
	t_cmd	*command;
	char	**str;

	command = ft_lstnew_cmd();
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(command));
	i = count_word(list);
	str = NULL;
	if (i > 1)
	{
		str = allocate_str(i, str, command, garbage);
		current = *list;
		fill_command(current, str, command, garbage);
	}
	else
	{
		current = *list;
		fill_command2(current, str, command, garbage);
	}
	ft_lstadd_back_cmd(cmd, command);
}

void	fill_cmd2(t_cmd **cmd, t_elem **list, t_garbage **garbage)
{
	t_elem	*current;

	current = *list;
	while (current)
	{
		current = fill_argc(cmd, &current, garbage);
		if (current && current->type == PIPE)
			current = current->next;
	}
}

t_elem	*fill_command_pipe(t_elem *current, char **str, t_cmd *command, t_garbage **grb)
{
	int	j;
	int	argc;

	(1) && (j = 0, argc = 0, 0);
	while (current && current->type != PIPE)
	{
		if (current->type >= REDIR_IN && current->type <= APPEND)
			ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi
				(current->content, current->type, current->fd, grb));
		else if (current->type == WORD)
		{
			if (current && current->content && j == 0 && str[argc])
			{
				command->cmd = current->content;
				str[argc++] = current->content;
				j++;
			}
			else if (current && current->content)
			{
				str[argc++] = current->content;
			}
		}
		current = current->next;
	}
	return (str[argc] = NULL, current);
}

t_elem	*fill_command_pipe2(t_elem *current, char **str, t_cmd *command,
		t_garbage **grb)
{
	while (current && current->type != PIPE)
	{
		if (current->type == WORD)
		{
			command->cmd = current->content;
			str = (char **)malloc(sizeof(char *) * (2));
			if (!str)
				return (NULL);
			ft_lstadd_back_garbage(grb, ft_lstnew_garbage(str));
			command->argc = str;
			str[0] = current->content;
			str[1] = NULL;
		}
		if (current->type >= REDIR_IN && current->type <= APPEND)
			ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi
				(current->content, current->type, current->fd, grb));
		current = current->next;
	}
	return (current);
}

t_elem	*fill_argc(t_cmd **cmd, t_elem **list, t_garbage **garbage)
{
	int		i;
	t_elem	*current;
	t_elem	*next;
	t_cmd	*command;
	char	**str;

	command = ft_lstnew_cmd();
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(command));
	i = count_word2(list);
	str = NULL;
	if (i > 1)
	{
		str = allocate_str(i, str, command, garbage);
		current = *list;
		next = fill_command_pipe(current, str, command, garbage);
	}
	else
	{
		current = *list;
		next = fill_command_pipe2(current, str, command, garbage);
	}
	ft_lstadd_back_cmd(cmd, command);
	return (next);
}

void	rename_token(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	while (current)
	{
		if (current->type < S_PACE)
			current->type = WORD;
		current = current->next;
	}
}

int	word_count(t_elem *list)
{
	t_elem	*current;
	int		i;

	current = list;
	i = 0;
	while (current)
	{
		if (current->type == WORD)
			i++;
		if (current->type == PIPE)
			break ;
		current = current->next;
	}
	return (i);
}

// void	concatination(t_elem **list, t_garbage **garbage)
// {
// 	t_elem	*new_list;
// 	char	*str;
// 	t_elem	*current;
// 	t_elem	*holder;

// 	if (!*list)
// 		return ;
// 	new_list = NULL;
// 	current = *list;
// 	while (current)
// 	{
// 		if (current && current->type < S_PACE) // new_list, current, 
// 		{
// 			if (!current->content)
// 			{
// 				ft_lstadd_back(&new_list, ft_lstnew(NULL, current->type, garbage));
// 				current = current->next;
// 				continue ;
// 			}
// 			str = ft_strdup("", garbage);
// 			while (current && current->type < S_PACE)
// 			{
// 				if (current->content)
// 					str = ft_strjoin(str, current->content, garbage);
// 				current = current->next;
// 			}
// 			ft_lstadd_back(&new_list, ft_lstnew(str, WORD, garbage));
// 			str = NULL;
// 		}
// 		else
// 		{
// 			if (current->fd != -1)
// 			{
// 				holder = ft_lstnew(current->content, current->type, garbage);
// 				holder->fd = current->fd;
// 				ft_lstadd_back(&new_list, holder);
// 			}
// 			else
// 				ft_lstadd_back(&new_list, ft_lstnew(current->content, current->type, garbage));
// 			current = current->next;
// 		}
// 	}
// 	if (new_list)
// 		*list = new_list;
// }

char	*concatinate_string(t_elem **current, t_garbage **garbage)
{
	char	*str;

	str = ft_strdup("", garbage);
	while (*current && (*current)->type < S_PACE)
	{
		if ((*current)->content)
			str = ft_strjoin(str, (*current)->content, garbage);
		*current = (*current)->next;
	}
	return (str);
}

void	concatinate(t_elem **current, t_elem **new_list, t_garbage **grb)
{
	char	*str;

	str = NULL;
	if (!(*current)->content)
	{
		ft_lstadd_back(new_list, ft_lstnew(NULL, (*current)->type, grb));
		*current = (*current)->next;
		return ;
	}
	str = concatinate_string(current, grb);
	ft_lstadd_back(new_list, ft_lstnew(str, WORD, grb));
}

void	concatinate_the_rest(t_elem **new_list, t_elem *current, t_garbage **grb)
{
	t_elem	*new_elem;

	if (current->fd != -1)
	{
		new_elem = ft_lstnew(current->content, current->type, grb);
		new_elem->fd = current->fd;
	}
	else
		new_elem = ft_lstnew(current->content, current->type, grb);
	ft_lstadd_back(new_list, new_elem);
}

void	concatination(t_elem **list, t_garbage **garbage)
{
	t_elem	*new_list;
	t_elem	*current;

	if (!*list)
		return ;
	new_list = NULL;
	current = *list;
	while (current)
	{
		if (current && current->type < S_PACE)
			concatinate(&current, &new_list, garbage);
		else
		{
			concatinate_the_rest(&new_list, current, garbage);
			current = current->next;
		}
	}
	if (new_list)
		*list = new_list;
}
