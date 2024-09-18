/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:33:33 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/18 18:10:31 by ykasmi           ###   ########.fr       */
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
		if (var->list->argc[i][0] && (var->list->argc[i][0] == '_' || ft_isalpha(var->list->argc[i][0])))
		{
			while (curr)
			{
				if (ft_strcmp(curr->key, var->list->argc[i]) == 0)
				{
					if (prev)
						prev->next = curr->next;
					else
						var->env = var->env->next;
					free(curr->key);
					free(curr->value);
					free(curr);
					break;
				}
				prev = curr;
				curr = curr->next;
			}
			prev = NULL;
			curr = var->env;
		}
		else
			env_key_error(var->list->argc, &var->env, i, "unset");
	}
}

//SECURITYSESSIONID COMMAND_MODE LaunchInstanceID OLDPWD ZSH HOMEBREW_PREFIX VSCODE_GIT_ASKPASS_EXTRA_ARGS VSCODE_GIT_ASKPASS_MAIN
