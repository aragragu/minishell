/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:15:38 by aragragu          #+#    #+#             */
/*   Updated: 2024/08/20 17:54:40 by aragragu         ###   ########.fr       */
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
        if (token && token->type == VAR){
            
            expand_var(&token->content, env, garbage);
            
        }
        else if (token && token->type == D_QOUTS){
            
            expand_d_qouts(env, &token->content, garbage);
        }
        token = token->next;
    }
}

t_env *fill_env(char **str, t_garbage **garbage)
{
    char *key;
    char *value;
    int end;
    int i;
    int j;
    t_env *list = NULL;

    j = 0;
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
                ft_lstadd_back2(&list, ft_lstnew2(key, value, garbage));
                break;
            }
            i++;
        }
        j++;
    }
    return (list);
}

void expand_var(char **str, t_env **env, t_garbage **garbage)
{
    int i = 0;
    char *gtr = *str;
    if (gtr[i] == '$')
    {
        t_env *list = *env;
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
                    return;
                }
                list = list->next;
            }
            *str = NULL;
        }
    }
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
            expand_var(&current->content, env, garbage);
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

    // list = token_quots(&list, *ptr, garbage);
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