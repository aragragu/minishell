/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 10:44:37 by aragragu          #+#    #+#             */
/*   Updated: 2024/09/08 19:10:50 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void read_input(char **env)
{
    t_elem *list = NULL;
    t_env *env_list = NULL;
    t_garbage *garbage = NULL;
    t_garbage *garb = NULL;
    t_cmd *cmd = NULL;
    t_var var;
    var.env = NULL;
    var.list = NULL;
    char *input;
    // env_list = fill_env(env, &garb);
    // var.env = env_list;
    init_env(&var.env, env);
    // var.env = env_list;
    // var.list = malloc(sizeof(t_cmd));
    // print_env_list(env_list);
    while (1)
    {
        input = readline("➜ minishell💀$ ");
        if (!input)
            break;
        if (!*input)
        {
            free(input);
            continue;
        }
        ft_lstadd_back_garbage(&garbage, ft_lstnew_garbage(input));
        add_history(input);
        list = token_input(&list, &input, &garbage);
        if (!sysntax_error_checker(&garbage, input, &list))
            continue;
        // print_list(&list);
        handle_redirection(&list, &env_list, &garbage);
        expand_var_list(&list, &env_list, &garbage);
        concatination(&list, &garbage);
        import_data(&cmd, &list, &garbage);
        var.list = cmd;
        print_cmd(cmd);
        if (check_builtins(cmd->cmd))
			ft_builtins(&var, cmd->cmd, &cmd);
        // print_env_list(env_list);
        // puts("==================================================");
        // print_env_list(var.env);
		// if (ft_strcmp(inp, "env") == 0)
		// 	ft_env(&var.env);  
        // print_cmd(cmd);
        free_garbage(&garbage);
        list = NULL;
        garbage = NULL;
        cmd = NULL;
    }
    free_garbage(&garb);
}

int not_special(char c)
{
    if (c == '|' || c == '>' || c == '<' || c == '$' || c == '\"' || c == '\'' || c == ' ')
        return (0);
    return (1);
}

t_elem *token_input(t_elem **list, char **in, t_garbage **garbage)
{
    int i = 0;
    char *input;

    input = ft_strtrim(*in, " \t\n\v\f\r", garbage);
    while (input && input[i])
    {
        if (input[i] && is_withespace(input[i]))
        {
            while (input[i] && is_withespace(input[i]))
                i++;
            i--;
            ft_lstadd_back(list, ft_lstnew(ft_strdup(" ", garbage), SPACE, garbage));
        }
        else if (input[i] == '>' && input[i + 1] == '>')
            ft_lstadd_back(list, ft_lstnew(ft_strdup(">>", garbage), APPEND, garbage));
        else if (input[i] == '<' && input[i + 1] == '<')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("<<", garbage), HEREDOC, garbage));
        else if (input[i] == '<')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("<", garbage), REDIR_IN, garbage));
        else if (input[i] == '>')
            ft_lstadd_back(list, ft_lstnew(ft_strdup(">", garbage), REDIR_OUT, garbage));
        else if (input[i] == '$' && input[i + 1] == '$')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("$$", garbage), DOUBLE_DLR, garbage));
        else if (input[i] == '$' && (input[i + 1] == '\"' || input[i + 1] == '\''))
        {
            i++;
            continue;
        }
        else if (input[i] == '$')
            is_a_var(list, input, i, garbage);
        else if (input[i] == '|')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("|", garbage), PIPE, garbage));
        else if (input[i] == '\"')
            is_a_quot(list, input, i, garbage);
        else if (input[i] == '\'')
            is_a_squot(list, input, i, garbage);
        else if (input[i] == '(')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("(", garbage), OPENING_PARENTHESIS, garbage));
        else if (input[i] == ')')
            ft_lstadd_back(list, ft_lstnew(ft_strdup(")", garbage), CLOSING_PARENTHESIS, garbage));
        else if (input[i] == '#')
            ft_lstadd_back(list, ft_lstnew(ft_strdup("#", garbage), HASH, garbage));
        else if (not_special(input[i]))
            is_a_word(list, input, i, garbage);
        i += ft_strlen(ft_lstlast(*list)->content);
    }
    return (*list);
}

void is_a_word(t_elem **list, char *input, int index, t_garbage **garbage)
{
    char *word;
    int len = 0;
    while (input[index + len] && not_special(input[index + len]) && !is_withespace(input[index + len]))
        len++;
    word = ft_substr(input, index, len, garbage);
    ft_lstadd_back(list, ft_lstnew(word, WORD, garbage));
}

void is_a_quot(t_elem **list, char *input, int index, t_garbage **garbage)
{
    int len = 0;
    char *str;
    while (input[index + 1 + len] && input[index + 1 + len] != '\"')
        len++;
    str = ft_substr(input, index, len + 2, garbage);
    ft_lstadd_back(list, ft_lstnew(str, D_QOUTS, garbage));
}

void is_a_squot(t_elem **list, char *input, int index, t_garbage **garbage)
{
    int len = 0;
    char *str;
    while (input[index + 1 + len] && input[index + 1 + len] != '\'')
        len++;
    str = ft_substr(input, index, len + 2, garbage);
    ft_lstadd_back(list, ft_lstnew(str, S_QOUTS, garbage));
}

void is_a_var(t_elem **list, char *input, int index, t_garbage **garbage)
{
    int len = 0;
    char *str;
    if (input[index + 1] >= '0' && input[index + 1] <= '9')
    {
        str = ft_substr(input, index, 2, garbage);
        ft_lstadd_back(list, ft_lstnew(str, VAR, garbage));
        return;
    }
    else
    {
        while (input[index + 1 + len] && (is_alphanumeric(input[index + 1 + len]) || input[index + 1 + len] == '_' || input[index + 1 + len] == '?'))
            len++;
        str = ft_substr(input, index, len + 1, garbage);
        if (ft_strlen(str) == 1)
            ft_lstadd_back(list, ft_lstnew(str, WORD, garbage));
        else
            ft_lstadd_back(list, ft_lstnew(str, VAR, garbage));
    }
}

void is_a_string(t_elem **list, char *input, int index, t_garbage **garbage)
{
    char *word;
    int len = 0;
    while (input[index + len] && input[index + len] != '$' && !is_withespace(input[index + len]))
        len++;
    word = ft_substr(input, index, len, garbage);
    ft_lstadd_back(list, ft_lstnew(word, WORD, garbage));
}

void edit_list(t_elem *list, t_garbage **garbage)
{
    char *str;
    if (list && list->type == D_QOUTS)
    {
        str = ft_strtrim(list->content, "\"", garbage);
        list->content = str;
    }
    if (list && list->type == S_QOUTS)
    {
        str = ft_strtrim(list->content, "\'", garbage);
        list->content = str;
    }
    list = list->next;
}

int is_special_character(char c)
{
    if ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126))
        return (1);
    return (0);
}