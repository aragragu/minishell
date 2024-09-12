/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 13:19:54 by aragragu          #+#    #+#             */
/*   Updated: 2024/09/11 12:00:35 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int sysntax_error_checker(t_garbage **garbage, char *str, t_elem **list)
{
    if (str && *list)
    {
        if (!has_unclosed_quots(str))
        {
            my_perror(garbage, list, "Error: Unclosed quotes\n");
            return (0);
        }
        else if (!has_logical_operators(str))
        {
            my_perror(garbage, list, "syntax error: logical operators not supported\n");
            return (0);
        }
        else if (has_logical_operators(str) == 2)
        {
            my_perror(garbage, list, "syntax error: single '&' not supported\n");
            return (0);
        }
        else if (!pipe_error(list))
        {
            my_perror(garbage, list, "error: syntax error near unexpected token `|'\n");
            return (0);
        }
        else if (!has_invalid_redirection_in(list))
        {
            my_perror(garbage, list, "Error: syntax error near unexpected token `<'\n");
            return (0);   
        }
        else if (!has_invalid_redirection_out(list))
        {
            my_perror(garbage, list, "Error: syntax error near unexpected token `>'\n");
            return (0);
        }
        else if (!has_invalid_heredoc(list))
        {
            my_perror(garbage, list, "Error: syntax error near unexpected token `<<'\n");
            return (0);
        }
        else if (!has_invalid_append(list))
        {
            my_perror(garbage, list, "Error: syntax error near unexpected token `>>'\n");
            return (0);   
        }
    }
    return (1);
}

int is_withespace(char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
        return (1);
    return (0);
}

int is_alphanumeric(char c)
{
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return 1;
    return 0;
}

int has_unclosed_quots(char *str)
{
    int i = 0;
    int sign = 0;
    while (str[i])
    {
        while (str[i] && is_withespace(str[i]))
            i++;
        ft_sign(str[i], &sign);
        i++;
    }
    if (sign == 1 || sign == 2)
        return (0);
    return (1);
}

int has_invalid_redirection_in(t_elem **list)
{
    t_elem *current = *list;
    if (ft_lstlast(*list)->type == REDIR_IN)
        return (0);
    while (current)
    {
        if (current && current->type == REDIR_IN)
        {
            if (current->next && current->next->type == SPACE)
            {
                current = current->next->next;
                if (current && (current->type >= PIPE))
                    return (0);
            }
            if (current->next && current->next->type >= PIPE)
                return (0);
        }
        current = current->next;
    }
    return (1);
}
int has_invalid_redirection_out(t_elem **list)
{
    t_elem *current = *list;
    t_elem *pah;
    if (ft_lstlast(*list)->type == REDIR_OUT)
        return (0);
    while (current)
    {
        if (current && current->type == REDIR_OUT)
        {
            if (current->next && current->next->type == SPACE)
            {
                pah = current->next->next;
                if (pah && pah->type >= PIPE)
                    return (0);
            }
            if (current->next && current->next->type >= PIPE)
                return (0);
        }
        current = current->next;
    }
    return (1);
}
int pipe_error(t_elem **head)
{
    t_elem *list = *head;
    t_elem *pah;
    if (list->type == PIPE)
        return (0);
    else if (ft_lstlast(list)->type == PIPE)
        return (0);
    while (list)
    {
        if (list && list->type == PIPE)
        {
            if (list->next && list->next->type == SPACE)
            {
                pah = list->next->next;
                if (pah && pah->type == PIPE)
                    return (0);
            }
            if (list->next && list->next->type == PIPE)
                return (0);
        }
        list = list->next;
    }
    return (1);
}

int has_invalid_heredoc(t_elem **list)
{
    t_elem *current = *list;
    t_elem *pah;
    if (ft_lstlast(*list)->type == HEREDOC)
        return (0);
    while (current)
    {
        if (current && current->type == HEREDOC)
        {
            if (current->next && current->next->type == SPACE)
            {
                pah = current->next->next;
                if (pah && pah->type >= PIPE)
                    return (0);
            }
            if (current->next && current->next->type >= PIPE)
                return (0);
        }
        current = current->next;
    }
    return (1);
}

int has_invalid_append(t_elem **list)
{
    t_elem *current = *list;
    t_elem *pah;
    if (ft_lstlast(*list)->type == APPEND)
        return (0);
    while (current)
    {
        if (current && current->type == APPEND)
        {
            if (current->next && current->next->type == SPACE)
            {
                pah = current->next->next;
                if (pah && pah->type >= PIPE)
                    return (0);
            }
            if (current->next && current->next->type >= PIPE)
                return (0);
        }
        current = current->next;
    }
    return (1);
}

int     has_logical_operators(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] && str[i] == '&')
        {
            if (str[i + 1] && str[i + 1] == '&')
                return (0);
            return (2);
        }
        if (str[i] && str[i] == '|')
        {
            if (str[i + 1] && str[i + 1] == '|')
                return (0);
        }
        i++;
    }
    return (1); 
}


void my_perror(t_garbage **garbage, t_elem **list, char *str)
{
    free_garbage(garbage);
    write(2, str, ft_strlen(str));
    *list = NULL;
    *garbage = NULL;
}