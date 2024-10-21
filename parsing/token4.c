/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:03:34 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 21:30:08 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_a_var(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*str;

	len = 0;
	if (input[index + 1] >= '0' && input[index + 1] <= '9')
	{
		str = ft_substr(input, index, 2, garbage);
		ft_lstadd_back(list, ft_lstnew(str, VAR, garbage));
		return ;
	}
	else
	{
		while (input[index + 1 + len] && (is_alphanumeric(
					input[index + 1 + len]) || input[index + 1 + len] == '_'))
			len++;
		str = ft_substr(input, index, len + 1, garbage);
		if (ft_strlen(str) == 1)
			ft_lstadd_back(list, ft_lstnew(str, WORD, garbage));
		else
			ft_lstadd_back(list, ft_lstnew(str, VAR, garbage));
	}
}

void	is_a_string(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*word;

	len = 0;
	while (input[index + len] && input[index + len] != '$')
		len++;
	word = ft_substr(input, index, len, garbage);
	ft_lstadd_back(list, ft_lstnew(word, WORD, garbage));
}

void	edit_list(t_elem *list, t_garbage **garbage)
{
	if (list && list->type == D_QOUTS)
		list->content = ft_strtrim(list->content, "\"", garbage);
	else if (list && list->type == S_QOUTS)
		list->content = ft_strtrim(list->content, "\'", garbage);
}

void	edit_all_list(t_elem **list, t_garbage **garbage)
{
	t_elem	*ptr;

	ptr = *list;
	while (ptr)
	{
		edit_list(ptr, garbage);
		ptr = ptr->next;
	}
}

void	free_garbage(t_garbage **garbage)
{
	t_garbage	*next;
	t_garbage	*current;

	if (!garbage)
		return ;
	current = *garbage;
	while (current)
	{
		next = current->next;
		if (current->value != NULL)
			free(current->value);
		free(current);
		current = next;
	}
	*garbage = NULL;
}
