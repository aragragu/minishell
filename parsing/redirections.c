/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 11:29:24 by aragragu          #+#    #+#             */
/*   Updated: 2024/09/26 16:45:06 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_redirection(t_elem **list, t_env **env,t_garbage **garbage)
{
    if (!*list)
        return;
    t_elem *current = *list;
    if (list && ((*list)->type == REDIR_OUT || (*list)->type == REDIR_IN ||
                 (*list)->type == HEREDOC || (*list)->type == APPEND))
        starting_red(list, env, garbage);
    while (current)
    {
        if (current->next && current->next->type == REDIR_OUT)
            redirection_out_list(&current);
        else if (current->next && current->next->type == REDIR_IN)
            redirection_in_list(&current);
        else if (current->next && current->next->type == HEREDOC)
            herdoc_list(&current, env, garbage);
        else if (current->next && current->next->type == APPEND)
            append_list(&current);
        current = current->next;
    }
}

void starting_red(t_elem **list, t_env **env,t_garbage **garbage)
{
    t_elem *current = *list;
    t_elem *holder;
    if (current && current->type == REDIR_IN)
    {
        if (current->next && current->next->type == SPACE)
        {
            holder = current->next;
            if (holder->next && holder->next->type < SPACE)
            {
                *list = holder->next;
                holder->next->type = REDIR_IN;
                return;
            }
        }
        else if (current->next && current->next->type < SPACE)
        {
            *list = current->next;
            current->next->type = REDIR_IN;
            return;
        }
    }
    else if (current && current->type == REDIR_OUT)
    {
        if (current->next && current->next->type == SPACE)
        {
            holder = current->next;
            if (holder->next && holder->next->type < SPACE)
            {
                *list = holder->next;
                holder->next->type = REDIR_OUT;
                return;
            }
        }
        else if (current->next && current->next->type < SPACE)
        {
            *list = current->next;
            current->next->type = REDIR_OUT;
            return;
        }
    }
    else if (current && current->type == APPEND)
    {
        if (current->next && current->next->type == SPACE)
        {
            holder = current->next;
            if (holder->next && holder->next->type < SPACE)
            {
                *list = holder->next;
                holder->next->type = APPEND;
                return;
            }
        }
        else if (current->next && current->next->type < SPACE)
        {
            *list = current->next;
            current->next->type = APPEND;
            return;
        }
    }
    else if (current && current->type == HEREDOC)
    {
        if (current->next && current->next->type == SPACE)
        {
            holder = current->next;
            if (holder->next && holder->next->type < SPACE)
            {
                if (holder->next->type == VAR)
                    open_herdoc(&holder->next, env, garbage, 1);
                else 
                    open_herdoc(&holder->next, env, garbage, 1);
                *list = holder->next;
                return;
            }
        }
        else if (current->next && current->next->type < SPACE)
        {
            if (current->next->type == VAR)
                open_herdoc(&current->next, env, garbage, 1);
            else 
                open_herdoc(&current->next, env, garbage, 1);
            *list = current->next;
            return;
        }
    }
}

void redirection_out_list(t_elem **list)
{
    if (!*list)
        return;
    t_elem *current = *list;
    t_elem *red_out;
    t_elem *red_out2;
    if (current && current->next)
    {
        if (current->next->type == REDIR_OUT && !ft_strcmp(current->next->content, ">"))
        {
            red_out = current->next;
            if (red_out && red_out->next)
            {
                if (red_out->next && red_out->next->type < SPACE)
                {
                    red_out->next->type = REDIR_OUT;
                    current->next = red_out->next;
                    return;
                }
                else if (red_out->next->next && red_out->next->type == SPACE)
                {
                    red_out2 = red_out->next->next;
                    if (red_out2->type < SPACE)
                    {
                        red_out2->type = REDIR_OUT;
                        current->next = red_out2;
                        return;
                    }
                }
            }
        }
    }
}
void redirection_in_list(t_elem **list)
{
    if (!*list)
        return;
    t_elem *current = *list;
    t_elem *red_out;
    t_elem *red_out2;
    if (current && current->next)
    {
        if (current->next->type == REDIR_IN && !ft_strcmp(current->next->content, "<"))
        {
            red_out = current->next;
            if (red_out && red_out->next)
            {
                if (red_out->next && red_out->next->type < SPACE)
                {
                    red_out->next->type = REDIR_IN;
                    current->next = red_out->next;
                    return;
                }
                else if (red_out->next->next && red_out->next->type == SPACE)
                {
                    red_out2 = red_out->next->next;
                    if (red_out2->type < SPACE)
                    {
                        red_out2->type = REDIR_IN;
                        current->next = red_out2;
                        return;
                    }
                }
            }
        }
    }
}

void herdoc_list(t_elem **list, t_env **env,t_garbage **garbage)
{
    t_elem *current = *list;
    t_elem *herdoc;

    if (current->next && current->next->type == HEREDOC && !ft_strcmp(current->next->content, "<<"))
    {
        herdoc = current->next;
        if (herdoc->next && herdoc->next->type < SPACE)
        {
            if (herdoc->next->type == VAR)
                open_herdoc(&herdoc->next, env, garbage, 1);
            else 
                open_herdoc(&herdoc->next, env, garbage, 1);
            current->next = herdoc->next;
            return;
        }
        else if (herdoc->next && herdoc->next->type == SPACE)
        {
            if (herdoc->next->next && herdoc->next->next->type < SPACE)
            {
                if (herdoc->next->next->type == VAR)
                    open_herdoc(&herdoc->next->next, env, garbage, 1);
                else 
                    open_herdoc(&herdoc->next->next, env, garbage, 1);
                current->next = herdoc->next->next;
                return;
            }
        }
    }
}

void open_herdoc(t_elem **list, t_env **env,t_garbage **garbage, int flag)
{
    static int i;
    char *line;
    char *buffer = ft_strdup("", garbage);
    t_elem *current = *list;
    if (current->type == D_QOUTS || current->type == S_QOUTS)
        edit_list(current, garbage);
    char *temp;
    char *file_name = (ft_strjoin(ft_strdup("tmp_", garbage), ft_itoa(++i), garbage));
    int fd = 0;
    
    while (1)
    {
        line = readline(">");
        if (!line)
            break;
        if (!ft_strcmp(current->content, line))
        {
            fd = open(file_name, O_CREAT | O_RDWR, 0644);
            if (fd == -1)
            {
                perror("Error opening file");
                break;
            }
            write(fd, buffer, ft_strlen(buffer));
            close (fd);
            fd = open(file_name, O_CREAT | O_RDWR, 0644);
            // unlink(file_name);
            // close(fd);
            break;
        }
        if (!*line)
        {
            free(line);
            continue;
        }
        ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(line));
        if (flag)
            expand_d_qouts_2(env, &line, garbage);
        if (!line)
            continue ;
        temp = ft_strjoin(line, "\n", garbage);
        if (!temp)
            break;
        temp = ft_strjoin(buffer, temp, garbage);
        if (!temp)
            break;
        buffer = temp;
    }
    // printf("===[%d]====\n", fd);
    current->content = file_name;
    current->type = HEREDOC;
    current->fd = fd;
    // printf("===[%d]====\n", current->fd);
}

void append_list(t_elem **list)
{
    if (!*list)
        return;
    t_elem *current = *list;
    t_elem *red_out;
    t_elem *red_out2;
    if (current && current->next)
    {
        if (current->next->type == APPEND && !ft_strcmp(current->next->content, ">>"))
        {

            red_out = current->next;
            if (red_out && red_out->next)
            {
                if (red_out->next && red_out->next->type < SPACE)
                {
                    red_out->next->type = APPEND;
                    current->next = red_out->next;
                    return;
                }
                else if (red_out->next->next && red_out->next->type == SPACE)
                {
                    red_out2 = red_out->next->next;
                    if (red_out2->type < SPACE)
                    {
                        red_out2->type = APPEND;
                        current->next = red_out2;
                        return;
                    }
                }
            }
        }
    }
}

void    expand_herdoc(char **str, t_env **env, t_garbage **garbage)
{
    int i = 0;
    char *gtr = *str;
    if (gtr[i] == '$')
    {
        t_env   *list = *env;
        if (gtr[i + 1] >= '0' && gtr[i + 1] <= '9')
        {
            *str = ft_strdup("", garbage);
            return;
        }
        else
        {
            while (list)
            {
                if (!ft_strcmp(list->key, gtr + 1))
                {
                    *str = ft_strdup(list->value, garbage);
                    return ;
                }
                list = list->next;
            }
            *str = ft_strdup("", garbage);
        }
    }
}