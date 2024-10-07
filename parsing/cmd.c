/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:30:40 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/07 20:38:41 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void import_data(t_cmd **cmd, t_elem **list, t_garbage **garbage)
{
    if (!*list)
        return;
    t_elem *current = *list;
    int pipe = 0;
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

void fill_cmd1(t_cmd **cmd, t_elem **list, t_garbage **garbage)
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
        if (current->content && current->type == WORD)
            i++;
        current = current->next;
    }
    if (i > 1)
    {
        str = (char **)malloc(sizeof(char *) * (i + 1));
        if (!str)
            return;
        ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(str));
        str[i] = NULL;
        command->argc = str;
        current = *list;
        while (current)
        {
            if (current->type >= REDIR_IN && current->type <= APPEND)
                ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi(current->content, current->type, current->fd,  garbage));
            else if (current->type == WORD)
            {
                if (current && current->content && j == 0)
                {
                    command->cmd = current->content;
                    str[argc++] = current->content;
                    j++;
                }
                else if (current && current->content)
                {
                    str[argc] = current->content;
                    argc++;
                }
            }
            current = current->next;
        }
        str[argc] = NULL;
    }
    else
    {
        current = *list;
        while (current && current->type != PIPE)
        {
            if (current->type == WORD)
            {
                command->cmd = current->content;
                str = (char **)malloc(sizeof(char *) * (2));
                if (!str)
                    return;
                ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(str));
                command->argc = str;
                str[0] = current->content;
                str[1] = NULL;
            }
            if (current->type >= REDIR_IN && current->type <= APPEND)
            {
                // printf("=======[%d]\n", current->fd);
                ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi(current->content, current->type, current->fd,  garbage));
                // printf("=======[%d]\n", current->fd);
            }
            current = current->next;
        }
    }
    ft_lstadd_back_cmd(cmd, command);
}

void fill_cmd2(t_cmd **cmd, t_elem **list, t_garbage **garbage)
{
    t_elem *current = *list;
    while (current)
    {

        current = fill_argc(cmd, &current, garbage);
        if (current && current->type == PIPE)
            current = current->next;
    }
}
t_elem *fill_argc(t_cmd **cmd, t_elem **list, t_garbage **garbage)
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
            break;
        if (count->type == WORD)
            i++;
        count = count->next;
    }
    if (i > 1)
    {
        str = (char **)malloc(sizeof(char *) * (i + 1));
        if (!str)
            return (NULL);
        ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(str));
        str[i] = NULL;
        command->argc = str;
        while (current && current->type != PIPE)
        {
            if (current->type >= REDIR_IN && current->type <= APPEND)
                ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi(current->content, current->type, current->fd,  garbage));
            else if (current->type == WORD)
            {
                if (j == 0)
                {
                    command->cmd = current->content;
                    str[argc++] = current->content;
                    j++;
                }
                else if (current && current->content)
                {
                    str[argc] = current->content;
                    argc++;
                }
            }
            current = current->next;
        }
        str[argc] = NULL;
    }
    else
    {
        current = *list;
        while (current && current->type != PIPE)
        {
            if (current->type == WORD)
            {
                command->cmd = current->content;
                str = (char **)malloc(sizeof(char *) * 2);
                if (!str)
                    return (NULL);
                ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(str));
                command->argc = str;
                str[0] = current->content;
                str[1] = NULL;
            }
            if (current->type >= REDIR_IN && current->type <= APPEND)
                ft_lstadd_back_redi(&command->redirection, ft_lstnew_redi(current->content, current->type, current->fd,  garbage));
            current = current->next;
        }
    }
    ft_lstadd_back_cmd(cmd, command);
    return (current);
}

void rename_token(t_elem **list)
{
    if (!*list)
        return;
    t_elem *current = *list;
    while (current)
    {
        if (current->type < S_PACE)
            current->type = WORD;
        current = current->next;
    }
}

int word_count(t_elem *list)
{
    t_elem *current = list;
    int i = 0;
    while (current)
    {
        if (current->type == WORD)
            i++;
        if (current->type == PIPE)
            break;
        current = current->next;
    }
    return (i);
}

void concatination(t_elem **list, t_garbage **garbage)
{
    t_elem *new_list = NULL;
    if (!*list)
        return;
    t_elem *current = *list;
    t_elem *holder;
    char *str;
    while (current)
    {
        if (current && current->type < S_PACE)
        {
            if (!current->content)
            {
                ft_lstadd_back(&new_list, ft_lstnew(NULL, current->type, garbage));
                current = current->next;
                continue;
            }
            str = ft_strdup("", garbage);
            while (current && current->type < S_PACE)
            {
                if (current->content)
                    str = ft_strjoin(str, current->content, garbage);
                current = current->next;
            }
            ft_lstadd_back(&new_list, ft_lstnew(str, WORD, garbage));
            str = NULL;
        }
        else
        {
            if (current->fd != -1)
            {
                holder = ft_lstnew(current->content, current->type, garbage);
                holder->fd = current->fd;
                ft_lstadd_back(&new_list, holder);
            }
            else
                ft_lstadd_back(&new_list, ft_lstnew(current->content, current->type, garbage));
            current = current->next;
        }
    }
    if (new_list)
        *list = new_list;
}