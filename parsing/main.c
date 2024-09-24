/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 17:02:07 by aragragu          #+#    #+#             */
/*   Updated: 2024/09/24 18:58:56 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    f(){system("leaks minishell");}
const char* get_token_type_string(t_token type) {
    switch (type) {
        case WORD: return "WORD";
        case D_QOUTS: return "D_QOUTS";
        case S_QOUTS: return "S_QOUTS";
        case VAR: return "VAR";
        case DOUBLE_DLR: return "DOUBLE_DLR";
        case SPACE: return "SPACE";
        case PIPE: return "PIPE";
        case REDIR_IN: return "REDIR_IN";
        case REDIR_OUT: return "REDIR_OUT";
        case HEREDOC: return "HEREDOC";
        case APPEND: return "APPEND";
        default: return "UNKNOWN";
    }
}


void    print_list(t_elem **list) {
    if (!*list)
        return ;
    t_elem *current = *list;
    while (current != NULL) {
        printf("Content: (%s), Type: %s\n", current->content, get_token_type_string(current->type));
        current = current->next;
    }
}

void print_env_list(t_env *head)
{
    if (!head)
    {
        printf("Environment list is empty or not initialized.\n");
        return;
    }
    t_env *current = head;
    while (current)
    {
        printf("Key: %s, Value: %s\n", current->key, current->value);
        current = current->next;
    }
}

void    free_garbage(t_garbage **garbage)
{
    if (!garbage)
        return ;
    t_garbage *next;
    t_garbage *current = *garbage;
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

void print_cmd(t_cmd *cmd) 
{
    if (!cmd)
        return ;
    while (cmd) 
    {
        if (cmd->cmd)
            printf("Command: %s\n", cmd->cmd);
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
            t_redir *redir = cmd->redirection;
            while (redir) 
            {
                printf("  Redirection: [%s] (Type: %s) and fd = [%d]\n", redir->value, get_token_type_string(redir->type), redir->fd);
                redir = redir->next;
            }
        } 
        else 
            printf("Redirections: (null)\n");
        cmd = cmd->next;
    }
}


void    print_array(char **ptr)
{
    int i = 0;
    while (ptr[i])
    {
        printf("%s and i = %d\n", ptr[i], i);
        i++;
    }
}

int main(int argc, char *argv[], char **env)
{
    (void)argv;
    // atexit(f);
    if (argc == 1)
        read_input(env);
    else
        printf("no argument needed\n");
    return (0);
}
