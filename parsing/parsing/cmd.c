/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:30:40 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 20:45:21 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	fill_command(t_elem *current, char **str, t_cmd *command,
t_garbage **grb)
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

