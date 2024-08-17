/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:30:40 by aragragu          #+#    #+#             */
/*   Updated: 2024/08/16 15:31:30 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void        import_data(t_cmd **cmd, t_elem **list, t_garbage **garbage)
{
    if (!*list)
        return ;
    t_elem *current = *list;
    int pipe = 0;
    rename_token(list);
    while (current)
    {
        if (current->type == PIPE)
            pipe++;
        current = current->next;
    }
    if (pipe == 0)
        fill_cmd1(cmd, list, garbage);
    else
        fill_cmd2(cmd, list, garbage);
}

void        fill_cmd1(t_cmd  **cmd, t_elem **list, t_garbage **garbage)
{
    int i = 0;
    int j = 0;
    int argc = 0;
    t_elem *current = *list;
    t_cmd *command = ft_lstnew_cmd();
    ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(command));
    char **str;
    while (current)
    {
        if (current->type == WORD)
            i++;
        current = current->next;
    }
    if (i > 1)
    {
        str = (char **)malloc(sizeof(char *) * (i));
        if (!str)
            return ;
        ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(str));
        str[i - 1] = NULL;
        command->argc = str;
    }
    current = *list;
    while (current)
    {
        if (current->type >= REDIR_IN && current->type <= APPEND)
            ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi(current->content, current->type, garbage));
        else if (current->type == WORD)
        {
            if (current && j == 0)
            {
                command->cmd = current->content;
                j++;
            }
            else
            {
                str[argc] = current->content;
                argc++;
            }
        }
        current = current->next;
    }
    ft_lstadd_back_cmd(cmd, command);
}

void    fill_cmd2(t_cmd  **cmd, t_elem **list, t_garbage **garbage)
{
    t_elem *current = *list;
    while (current)
    {
        current = fill_argc(cmd ,&current, garbage);
        if (current && current->type == PIPE)
            current = current->next;
    }
}
t_elem    *fill_argc(t_cmd  **cmd, t_elem **list, t_garbage **garbage)
{
    int i = 0;
    int j = 0;
    int argc = 0;
    char **str;
    t_elem *count = *list;
    t_cmd *command = ft_lstnew_cmd();
    ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(command));
    t_elem *current = *list;
    while (count)
    {
        if (count->type == PIPE)
            break ;
        if (count->type == WORD)
            i++;
        count = count->next;
    }
    if (i > 1)
    {
        str = (char **)malloc(sizeof(char *) * (i));
        if (!str)
            return (NULL);
        ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(str));
        str[i - 1] = NULL;
        command->argc = str;
    }
    while (current && current->type != PIPE)
    {
        if (current->type >= REDIR_IN && current->type <= APPEND)
            ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi(current->content, current->type, garbage));
        else if (current->type == WORD)
        {
            if (j == 0)
            {
                command->cmd = current->content;
                j++;
            }
            else
            {
                str[argc] = current->content;
                argc++;
            }
        }
        current = current->next;
    }
    ft_lstadd_back_cmd(cmd, command);
    return (current);
}

void    rename_token(t_elem **list)
{
    if (!*list)
        return ;
    t_elem *current = *list;
    while (current)
    {
        if (current->type < SPACE)
            current->type = WORD;
        current = current->next;
    }
}

int     word_count(t_elem *list)
{
    t_elem *current = list;
    int i = 0;
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

