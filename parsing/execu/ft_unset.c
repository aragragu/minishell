/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:33:33 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/14 11:55:20 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(t_var *var)
{
	t_env	*prev;
	t_env	*curr;
	int		i;

	prev = NULL;
	curr = var->env;
	i = 0;
	while (var->list->argc[++i])
	{
		prev = NULL;
		curr = var->env;
		while (curr)
		{
			if (ft_strcmp(curr->key, var->list->argc[i]) == 0)
			{
				if (prev)
					prev->next = curr->next;
				else
					var->env = curr->next;
				free(curr->key);
				free(curr->value);
				free(curr);
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
	}
}

// void	ft_unset(t_var *var)
// {
// 	t_env	*prev;
// 	t_env	*curr;
// 	int		i;

// 	i = 0;
// 	while (var->list->argc[++i]) // Loop through all arguments
// 	{
// 		prev = NULL;
// 		curr = var->env;
// 		while (curr) // Loop through the environment variables
// 		{
// 			if (ft_strcmp(curr->key, var->list->argc[i]) == 0) // If a match is found
// 			{
// 				if (prev)
// 					prev->next = curr->next;
// 				else
// 					var->env = curr->next;

// 				// Free memory for the removed environment variable
// 				free(curr->key);
// 				free(curr->value);
// 				free(curr);

// 				// Break out of the inner loop after removal
// 				break;
// 			}
// 			prev = curr;
// 			curr = curr->next;
// 		}
// 	}
// }