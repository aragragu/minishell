/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:41:22 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 20:41:58 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
