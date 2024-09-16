/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 10:44:37 by aragragu          #+#    #+#             */
/*   Updated: 2024/09/16 22:04:14 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #define MAX_COMMANDS 100
// // #define MAX_ARGS 100

int calculate_num_cmds(char *input) {
    int num_cmds = 0;
    char *input_copy = strdup(input); // Make a copy of the input string

    // Use strtok to split the input by the pipe (|) character
    char *token = strtok(input_copy, "|");
    while (token != NULL) {
        num_cmds++;                   // Increment command count
        token = strtok(NULL, "|");     // Move to the next command
    }

    free(input_copy);                 // Free the duplicated string
    return num_cmds;                  // Return the total number of commands
}

char **parse_command(char *cmd) {
    char **args = malloc(8 * sizeof(char *));
    char *arg;
    int i = 0;

    arg = strtok(cmd, " ");
    while (arg != NULL) {
        args[i++] = arg;
        arg = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

void execute_pipe(char *input, int num_cmds, t_var *var) {
    int pipefd[2], prev_fd = 0;
    char *input_copy = strdup(input);
    (void)var;
    char *cmd = strtok(input_copy, "|");
    for (int i = 0; i < num_cmds; i++) {
        pipe(pipefd);
        if (fork() == 0) {
            dup2(prev_fd, 0); // read from prev_fd
            if (i < num_cmds - 1)
                dup2(pipefd[1], 1); // write to pipe if not the last command
            close(pipefd[0]);
            close(pipefd[1]);

            // Parse and execute the command
            char **args = parse_command(cmd);
            execvp(args[0], args);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        prev_fd = pipefd[0]; // next child reads from this pipe
        cmd = strtok(NULL, "|");
    }

    close(prev_fd);
    while (wait(NULL) > 0); // wait for all child processes
    free(input_copy);
}

// int calculate_num_cmds(char *input) {
//     int num_cmds = 0;
//     char *input_copy = strdup(input); // Make a copy of the input string

//     // Use strtok to split the input by the pipe (|) character
//     char *token = strtok(input_copy, "|");
//     while (token != NULL) {
//         num_cmds++;                   // Increment command count
//         token = strtok(NULL, "|");     // Move to the next command
//     }

//     free(input_copy);                 // Free the duplicated string
//     return num_cmds;                  // Return the total number of commands
// }

// Function to execute piped commands
// void execute_piped_commands(char *commands[], int num_cmds, t_var *var) {
//     int pipefds[2 * (num_cmds - 1)];
//     pid_t pids[MAX_CMDS];
//     int i;

//     // Create pipes
//     (void)commands;
//     for (i = 0; i < num_cmds - 1; i++) {
//         if (pipe(pipefds + i * 2) == -1) {
//             perror("pipe");
//             exit(EXIT_FAILURE);
//         }
//     }

//     for (i = 0; i < num_cmds; i++) {
//         pids[i] = fork();
//         if (pids[i] == 0) {
//             // Child process

//             // If not the first command, get input from previous pipe
//             if (i > 0) {
//                 if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) == -1) {
//                     perror("dup2 stdin");
//                     exit(EXIT_FAILURE);
//                 }
//             }

//             // If not the last command, send output to next pipe
//             if (i < num_cmds - 1) {
//                 if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) == -1) {
//                     perror("dup2 stdout");
//                     exit(EXIT_FAILURE);
//                 }
//             }

//             // Close all pipe file descriptors
//             for (int j = 0; j < 2 * (num_cmds - 1); j++) {
//                 close(pipefds[j]);
//             }

//             // Execute the command
//             ft_exc(var);  // Assuming `execute_single_command` runs the command
//         } else if (pids[i] < 0) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }
//     }

//     // Parent process closes all pipes
//     for (i = 0; i < 2 * (num_cmds - 1); i++) {
//         close(pipefds[i]);
//     }

//     // Parent waits for all children
//     for (i = 0; i < num_cmds; i++) {
//         waitpid(pids[i], NULL, 0);
//     }
// }

// Updated read_input function
void read_input(char **env) {
    t_elem *list = NULL;
    t_garbage *garbage = NULL;
    t_garbage *garb = NULL;
    t_var var;
    var.env = NULL;
    var.list = NULL;
    char *input;
    
    fill_env(&var.env, env, &garb);
    while (1) {
        input = readline("➜ minishell💀$ ");
        if (!input)
            break;
        if (!*input) {
            free(input);
            continue;
        }

        ft_lstadd_back_garbage(&garbage, ft_lstnew_garbage(input));
        add_history(input);
        list = token_input(&list, &input, &garbage);
        if (!list)
            continue;
        if (!sysntax_error_checker(&garbage, input, &list))
            continue;

        expand_var_list(&list, &var.env, &garbage);
        handle_redirection(&list, &var.env, &garbage);
        concatination(&list, &garbage);
        import_data(&var.list, &list, &garbage);
        if (check_builtins(var.list->cmd))
            ft_builtins(&var, var.list->cmd, &var.list);
        else if (ft_strcmp(input, "env") == 0)
            ft_env(&var.env);
        else if (access(var.list->cmd, X_OK) == 0)
            ft_exc2(&var);
        else if (check_valid_path(var.list->cmd, &var)) {
            if (check_valid_path(var.list->cmd, &var)) {
                int num_cmds = calculate_num_cmds(input);
                execute_pipe(input, num_cmds, &var);
}
            // int num_cmds = calculate_num_cmds(input);
            // execute_piped_commands(var.list->argc, num_cmds, &var);
            // ft_exc(&var);
        } else
            printf("minishell: %s: command not found\n", var.list->argc[0]);

        free_garbage(&garbage);
        list = NULL;
        garbage = NULL;
        var.list = NULL;
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
}

int is_special_character(char c)
{
    if ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126))
        return (1);
    return (0);
}