/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:18:50 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/22 00:48:30 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

const char	*get_token_type_string(t_token type)
{
	switch (type)
	{
		case WORD: return "WORD";
		case D_QOUTS: return "D_QOUTS";
		case S_QOUTS: return "S_QOUTS";
		case VAR: return "VAR";
		case DOUBLE_DLR: return "DOUBLE_DLR";
		case S_PACE: return "S_PACE";
		case PIPE: return "PIPE";
		case REDIR_IN: return "REDIR_IN";
		case REDIR_OUT: return "REDIR_OUT";
		case HEREDOC: return "HEREDOC";
		case APPEND: return "APPEND";
		case TILDE: return "TILDE";
		case AND: return "AND";
		default: return "UNKNOWN";
	}
}

void	print_list(t_elem **list)
{
	t_elem	*current;

	if (!*list)
		return ;
	current = *list;
	while (current != NULL)
	{
		printf("Content: [%s], Type: [%s], fd : %d\n", current->content,
			get_token_type_string(current->type), current->fd);
		current = current->next;
	}
}

void	print_env_list(t_env *head)
{
	t_env	*current;

	if (!head)
	{
		printf("Environment list is empty or not initialized.\n");
		return ;
	}
	current = head;
	while (current)
	{
		printf("Key: %s, Value: %s\n", current->key, current->value);
		current = current->next;
	}
}

void    print_cmd(t_cmd *cmd)
{
	t_redir	*redir;

	if (!cmd)
		return ;
	while (cmd)
	{
		if (cmd->cmd)
			printf("Command: [%s]\n", cmd->cmd);
		else
			printf("Command: (null)\n");
		if (cmd->argc && cmd->argc[0])
		{
			printf("Arguments:\n");
			for (int i = 0; cmd->argc[i] != NULL; i++) 
			{
				if (cmd->argc[i])
					printf("  Arg[%d]: [%s]\n", i, cmd->argc[i]);
			}
		}
		else
			printf("Arguments: (null)\n");
		if (cmd->redirection)
		{
			printf("Redirections:\n");
			redir = cmd->redirection;
			while (redir)
			{
				printf("  Redirection: [%s] (Type: %s) and fd = [%d]\n", redir->value,
					get_token_type_string(redir->type), redir->fd);
				redir = redir->next;
			}
		}
		else
			printf("Redirections: (null)\n");
		cmd = cmd->next;
	}
}

void	print_array(char **ptr)
{
	int i;

	i = 0;
	while (ptr[i])
	{
		printf("%s and i = %d\n", ptr[i], i);
		i++;
	}
}