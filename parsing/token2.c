/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:01:25 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/23 14:29:37 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_delemiter(t_elem **list, t_garbage **garbage)
{
	t_elem	*ptr;
	t_elem	*str;

	ptr = *list;
	str = NULL;
	while (ptr)
	{
		if (!ptr->ignore[0])
		{
			ft_lstadd_back(&str, ft_lstnew(ptr->content, ptr->type, garbage));
			ft_lstlast(str)->fd = ptr->fd;
			ft_lstlast(str)->fd_here = ptr->fd_here;
		}
		ptr = ptr->next;
	}
	*list = str;
}

int	fill_linked_list(char *input, t_var *var)
{
	int	i;
	int	j;

	i = fill_and_check(input, var);
	if (i == 1 || i == 2)
	{
		if (i == 1)
			return (1);
		if (i == 2)
			return (2);
	}
	j = list_handler(var);
	if (j == 1 || j == 2)
	{
		if (j == 1)
			return (1);
		if (j == 2)
			return (2);
	}
	return (0);
}

int	list_handler(t_var *var)
{
	edit_all_list(&var->linked_list, &var->garbage);
	ft_heredoc(var);
	expand_var_list(&var->linked_list, *var, &var->garbage);
	concatination(&var->linked_list, &var->garbage);
	handle_redirection(&var->linked_list);
	if (check_fd_her(&var->linked_list))
	{
		free_garbage(&var->garbage);
		g_es(1, 0);
		var->linked_list = NULL;
		var->garbage = NULL;
		var->list = NULL;
		return (2);
	}
	if (g_sig == 2)
	{
		free_garbage(&var->garbage);
		g_es(1, 0);
		var->linked_list = NULL;
		var->garbage = NULL;
		var->list = NULL;
		return (2);
	}
	import_data(&var->list, &var->linked_list, &var->garbage);
	return (0);
}

void	token_input(char **in, t_var *var)
{
	int		i;
	int		x;
	char	*input;

	i = 0;
	input = ft_strtrim(*in, " \t\n\v\f\r", &var->garbage);
	while (input && input[i])
	{
		handle_whitespace(input, &i, &var->linked_list, &var->garbage);
		x = handle_special_characters(input, &i, var);
		if (x)
			continue ;
		i += ft_strlen(ft_lstlast((*var).linked_list)->content);
	}
}

void	handle_whitespace(char *input, int *i, t_elem **list,
															t_garbage **garbage)
{
	if (input[*i] && is_whitespace(input[*i]))
	{
		while (input[*i] && is_whitespace(input[*i]))
			(*i)++;
		(*i)--;
		ft_lstadd_back(list, ft_lstnew(ft_strdup(" ", garbage),
				S_PACE, garbage));
	}
}
