/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:33:19 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 20:40:50 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_command2(t_elem *current, char **str, t_cmd *command,
		t_garbage **grb)
{
	while (current)
	{
		if (current->content && current->type == WORD)
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

t_elem	*fill_command_pipe(t_elem *current, char **str, t_cmd *command,
t_garbage **grb)
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
		if (current->content && current->type == WORD)
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
