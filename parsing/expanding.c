/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:15:38 by aragragu          #+#    #+#             */
/*   Updated: 2024/09/16 14:51:08 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expand_var_list(t_elem **list, t_env **env, t_garbage **garbage)
{
    t_elem *token;
    if (!*list)
        return;
    token = *list;
    while (token) // content -> var -> content
    {
        edit_list(token, garbage);
        if (token && token->type == HEREDOC)
        {
            if (token->next && token->next->type == SPACE)
            {
                if (token->next->next && (token->next->next->type == VAR || token->next->next->type == S_QOUTS || token->next->next->type == D_QOUTS))
                {
                    if (token->next->next->next)
                        token = token->next->next->next;
                    else
                        break;
                }
            }
            else
            {
                if (token->next && (token->next->type == VAR || token->next->type == S_QOUTS || token->next->type == D_QOUTS))
                {
                    if (token->next->next)
                        token = token->next->next;
                    else
                        break;
                }
            }
        }
        else if (token && token->type == VAR){
            expand_var(list, token, env, garbage);
        }
        else if (token && token->type == D_QOUTS)
            expand_d_qouts(env, &token->content, garbage);
        token = token->next;
    }
}

void    fill_env(t_env **env, char **str, t_garbage **garbage)
{
    char *key;
    char *value;
    int end;
    int i;
    int j;
    // t_env *list = NULL;

    j = 0;
	if (!str || !str[0])
	{
        *env = ft_lstnewww(ft_strduppp("PWD"), ft_strduppp("/Users/ykasmi/Desktop/parss/parsing"));//protection
		ft_lstadd_backkk(env, ft_lstnewww(ft_strduppp("SHLVL"), ft_strduppp("1")));
		ft_lstadd_backkk(env, ft_lstnewww(ft_strduppp("_"), ft_strduppp("/usr/bin/env")));
		return;
	}
    while (str[j])
    {
        i = 0;
        end = ft_strlen(str[j]);
        while (str[j][i])
        {
            if (str[j][i] && str[j][i] == '=')
            {
                key = ft_substr(str[j], 0, i, garbage);
                value = ft_substr(str[j] + i + 1, 0, (end - i - 1), garbage);
                ft_lstadd_back2(env, ft_lstnew2(key, value, garbage));
                break;
            }
            i++;
        }
        j++;
    }
    // return (list);
}

void expand_var(t_elem **elem ,t_elem *node, t_env **env, t_garbage **garbage)
{
    int i = 0;
    char *gtr = node->content;
    // t_elem *last = node->next;
    int flag = 0;

    if (gtr[i] == '$')
    {
        t_env *list = *env;
        if (gtr[i + 1] >= '0' && gtr[i + 1] <= '9')
        {
            node->content = ft_strdup("", garbage);
            return;
        }
        else
        {
            while (list)
            {
                if (!ft_strcmp(list->key, gtr + 1))
                {
                    node->content = ft_strdup(list->value, garbage);
                    flag = 1;
                    break;
                }
                list = list->next;
            }
            if (!flag)
                node->content = NULL;
        }
    }
    if (node->content && ft_strchr(node->content, ' '))
        ft_split_var(elem, node, garbage);
    
}

void expand_d_qouts(t_env **env, char **ptr, t_garbage **garbage)
{
    t_elem *list = NULL;
    t_elem *current = NULL;
    char *str;

    list = token_quots(&list, *ptr, garbage);
    current = list;
    str = ft_strdup("", garbage);
    while (current)
    {
        if (current->type == VAR)
            expand_var(&list, current, env, garbage);
        if (!current->content)
            current->content = ft_strdup("", garbage);
        str = ft_strjoin(str, current->content, garbage);
        current = current->next;
    }
    *ptr = str;
}

void expand_d_qouts_2(t_env **env, char **ptr, t_garbage **garbage)
{
    t_elem *list = NULL;
    t_elem *current = NULL;
    char *str;

    list = token_quots(&list, *ptr, garbage);
    current = list;
    str = ft_strdup("", garbage);
    while (current)
    {
        if (current->type == VAR)
            expand_herdoc(&current->content, env, garbage);
        if (!current->content)
            current->content = ft_strdup("", garbage);
        str = ft_strjoin(str, current->content, garbage);
        current = current->next;
    }
    *ptr = str;
}

t_elem *token_quots(t_elem **list, char *in, t_garbage **garbage)
{
    int i = 0;

    while (in && in[i])
    {
        if (in[i] && is_withespace(in[i]))
            ft_lstadd_back(list, ft_lstnew(ft_strdup(" ", garbage), SPACE, garbage));
        else if (in[i] == '$' && in[i + 1] == '$')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("$$", garbage), DOUBLE_DLR, garbage));
        else if (in[i] == '$')
            is_a_var(list, in, i, garbage);
        else if (in[i] && !is_withespace(in[i]) && in[i] != '$')
            is_a_string(list, in, i, garbage);
        i += ft_strlen(ft_lstlast(*list)->content);
    }
    return (*list);
}

void    ft_split_var(t_elem **elem, t_elem *node, t_garbage **garbage)
{
    t_elem *new_list = NULL;
    t_elem *prev = NULL;
    t_elem *current = *elem;
    t_elem *tmp;
    char **str = ft_split(node->content, ' ', garbage);
    int i = ft_strlen2(str);
    int j = 0;
    while (j < i)
    {
        ft_lstadd_back(&new_list, ft_lstnew(str[j], WORD, garbage));
        if (j != (i - 1))
            ft_lstadd_back(&new_list, ft_lstnew(ft_strdup(" ", garbage), SPACE, garbage));
        j++;
    }
    while (current)
    {
        if (!ft_strcmp(current->content, node->content))
        {
            tmp = ft_lstlast(new_list);
            if (current->next)
                tmp->next = current->next;
            if (prev)
                prev->next = new_list;
            else
                *elem = new_list;
        }
        else
            prev = current;
        current = current->next;
    }
}