/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:02:40 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 22:09:38 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_special_characters(char *input, int *i, t_var *var)
{
	if (input[*i] == '&')
		ft_lstadd_back(&var->linked_list, ft_lstnew(ft_strdup("&",
					&var->garbage), AND, &var->garbage));
	else if (input[*i] == '>' && input[*i + 1] == '>')
		ft_lstadd_back(&var->linked_list, ft_lstnew(ft_strdup(">>",
					&var->garbage), APPEND, &var->garbage));
	else if (input[*i] == '<' && input[*i + 1] == '<')
		ft_lstadd_back(&var->linked_list, ft_lstnew(ft_strdup("<<",
					&var->garbage), HEREDOC, &var->garbage));
	else if (input[*i] == '<')
		ft_lstadd_back(&var->linked_list, ft_lstnew(ft_strdup("<",
					&var->garbage), REDIR_IN, &var->garbage));
	else if (input[*i] == '>')
		ft_lstadd_back(&var->linked_list, ft_lstnew(ft_strdup(">",
					&var->garbage), REDIR_OUT, &var->garbage));
	else if (input[*i] == '$' && (input[*i + 1] == '\"'
			|| input[*i + 1] == '\''))
		(*i)++;
	else if (input[*i] == '$' && input[*i + 1] == '$')
		ft_lstadd_back(&var->linked_list, ft_lstnew(ft_strdup("$$",
					&var->garbage), DOUBLE_DLR, &var->garbage));
	else
		token_input_1(&var->linked_list, input, *i, var);
}

void	token_input_1(t_elem **list, char *input, int i, t_var *var)
{
	if (input[i] == '$' && input[i + 1] == '?')
		ft_lstadd_back(list, ft_lstnew(ft_strdup("$?",
					&var->garbage), EXIT_STATUS, &var->garbage));
	else if (input[i] == '$')
		is_a_var(list, input, i, &var->garbage);
	else if (input[i] == '\"')
		is_a_quot(list, input, i, &var->garbage);
	else if (input[i] == '\'')
		is_a_squot(list, input, i, &var->garbage);
	else if (input[i] == '|' && (g_es(0, 0), 1))
		ft_lstadd_back(list, ft_lstnew(ft_strdup("|",
					&var->garbage), PIPE, &var->garbage));
	else if (input[i] == '(')
		ft_lstadd_back(list, ft_lstnew(ft_strdup("(",
					&var->garbage), OPENING_PARENTHESIS, &var->garbage));
	else if (input[i] == '~' && input[i + 1] != '~')
		ft_lstadd_back(list, ft_lstnew(ft_strdup("~",
					&var->garbage), TILDE, &var->garbage));
	else if (input[i] == ')')
		ft_lstadd_back(list, ft_lstnew(ft_strdup(")",
					&var->garbage), CLOSING_PARENTHESIS, &var->garbage));
	else if (not_special(input[i]))
		is_a_word(list, input, i, &var->garbage);
}

void	is_a_word(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*word;

	len = 0;
	while (input[index + len] && not_special(input[index + len])
		&& !is_whitespace(input[index + len]))
		len++;
	word = ft_substr(input, index, len, garbage);
	ft_lstadd_back(list, ft_lstnew(word, WORD, garbage));
}

void	is_a_quot(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*str;

	len = 0;
	while (input[index + 1 + len] && input[index + 1 + len] != '\"')
		len++;
	str = ft_substr(input, index, len + 2, garbage);
	ft_lstadd_back(list, ft_lstnew(str, D_QOUTS, garbage));
}

void	is_a_squot(t_elem **list, char *input, int index, t_garbage **garbage)
{
	int		len;
	char	*str;

	len = 0;
	while (input[index + 1 + len] && input[index + 1 + len] != '\'')
		len++;
	str = ft_substr(input, index, len + 2, garbage);
	ft_lstadd_back(list, ft_lstnew(str, S_QOUTS, garbage));
}
