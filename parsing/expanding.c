/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:15:38 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 16:33:32 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	len(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13) || str[i] == '+')
		i++;
	if ((str[i] >= '0' && str[i] <= '9') || (str[i] == '-' && str[i] == '0'))
		i++;
	return (i);
}

static int	over_flow(int sign, const char *str)
{
	int	i;

	i = 0;
	while (str[i] == '+' || str[i] == '-')
		i++;
	if (len(str) > 20 && sign == -1)
		return (0);
	if (len(str) > 19 && sign == 1)
		return (-1);
	return (1);
}

int	ft_atoi2(char *str)
{
	size_t	i;
	int		n;
	size_t	result;

	i = 0;
	n = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			n = -n;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - 48);
		i++;
	}
	return (result * n);
}

void	expand_var_list(t_elem **list, t_var container, t_garbage **garbage)
{
	t_elem	*token;

	if (!list || !*list)
		return ;
	token = *list;
	while (token)
	{
		if (token && token->type == HEREDOC)
		{
			if (handle_herdoc_list(&token, garbage) == 1)
				break ;
		}
		if (token && token->type == VAR)
			expand_var(list, token, &container.env, garbage);
		else if (token && token->type == EXIT_STATUS)
			token->content = ft_itoa(g_es(0, 1), garbage);
		else if (token && token->type == D_QOUTS)
			expand_d_qouts(&container.env, &token->content, garbage);
		else if (token && token->type == TILDE)
			token->content = ft_getenv(container.env, "HOME");
		token = token->next;
	}
}

int	handle_herdoc_list(t_elem **token, t_garbage **garbage)
{
	int	i;

	if (token && *token && (*token)->type == HEREDOC)
	{
		if ((*token)->next && (*token)->next->type == S_PACE)
		{
			i = handle_herdoc_list2(token, garbage);
			if (i)
				return (1);
		}
		else
		{
			if ((*token)->next && ((*token)->next->type == VAR
					|| (*token)->next->type == S_QOUTS
					|| (*token)->next->type == D_QOUTS))
			{
				i = handle_herdoc_list3(token);
				if (i)
					return (1);
			}
		}
	}
	return (0);
}

int	handle_herdoc_list2(t_elem **token, t_garbage **garbage)
{
	(void )garbage;
	if ((*token)->next->next && ((*token)->next->next->type == VAR
			|| (*token)->next->next->type == S_QOUTS
			|| (*token)->next->next->type == D_QOUTS))
	{
		if ((*token)->next->next->next)
			*token = (*token)->next->next->next;
		else
			return (1);
	}
	return (0);
}

int	handle_herdoc_list3(t_elem **token)
{
	if ((*token)->next->next)
		*token = (*token)->next->next;
	else
		return (1);
	return (0);
}
void	searsh_env(t_env **env, char **str, t_garbage **grb, int end)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	j = 0;
	while (str[j])
	{
		i = 0;
		end = ft_strlen(str[j]);
		while (str[j][i])
		{
			if (str[j][i] && str[j][i] == '=')
			{
				key = ft_substr(str[j], 0, i, grb);
				value = ft_substr(str[j] + i + 1, 0, (end - i - 1), grb);
				if (!ft_strcmp(key, "SHLVL"))
					value = ft_strdup(ft_itoa(ft_atoi2(value) + 1, grb), grb);
				ft_lstadd_back2(env, ft_lstnew2(key, value, grb));
				break ;
			}
			i++;
		}
		j++;
	}
}

void	fill_env(t_env **env, char **str, t_garbage **garbage)
{
	int		end;

	end = 0;
	if (!str || !str[0])
	{
		*env = ft_lstnewww(ft_strduppp("PWD"), getcwd(NULL, 0));
		if (!*env)
			return ;
		ft_lstadd_backkk(env, ft_lstnewww(ft_strduppp("SHLVL"),
				ft_strduppp("1")));
		ft_lstadd_backkk(env, ft_lstnewww(ft_strduppp("_"),
				ft_strduppp("/usr/bin/env")));
		ft_lstadd_backkk(env, ft_lstnewww(ft_strduppp("OLDPWD"), NULL));
		return ;
	}
	searsh_env(env, str, garbage, end);
}

static int	has_a_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z'))
			return (1);
		i++;
	}
	return (0);
}

void	expand_var2(t_elem *node, char *gtr, t_garbage **grb, t_env *env)
{
	int		flag;

	flag = 0;
	while (env)
	{
		if (!ft_strcmp(env->key, gtr + 1))
		{
			if (env->value)
			{
				node->content = ft_strtrim(ft_strdup(env->value, grb), " \t\n\v\f\r", grb);
				flag = 1;
				break ;
			}
			else
				break ;
		}
		env = env->next;
	}
	if (!flag)
		node->content = NULL;
}

void	expand_var3(t_elem **list, t_elem *node, t_garbage **grb)
{
	t_elem	*checker;

	checker = *list;
	if (node->content)
	{
		if (has_a_char(node->content) && ft_strchr(node->content, ' ')
			&& !ft_strcmp((*checker).content, "export"))
		{
			if (checker->next && checker->next->type == S_PACE)
			{
				if (checker->next->next && checker->next->next->type == VAR
					&& !ft_strchr(checker->next->next->content, '='))
					ft_split_var(list, node, grb);
				else
					return ;
			}
		}
		else
			ft_split_var(list, node, grb);
	}
}

void	expand_var(t_elem **elem, t_elem *node, t_env **env, t_garbage **grb)
{
	int		i;
	char	*gtr;
	t_elem	*checker;
	int		flag;
	t_env	*list;

	checker = *elem;
	gtr = node->content;
	i = 0;
	flag = 0;
	list = *env;
	if (gtr[i] == '$')
	{
		if (gtr[i + 1] >= '0' && gtr[i + 1] <= '9')
		{
			node->content = ft_strdup("", grb);
			return ;
		}
		else
			expand_var2(node, gtr, grb, list);
	}
	expand_var3(elem, node, grb);
}

void	expand_d_qouts(t_env **env, char **ptr, t_garbage **garbage)
{
	t_elem		*list;
	t_elem		*current;
	char		*str;

	list = NULL;
	current = NULL;
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

void	expand_d_qouts_2(t_env **env, char **ptr, t_garbage **garbage)
{
	t_elem		*list;
	t_elem		*current;
	char		*str;

	list = NULL;
	current = NULL;
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

t_elem	*token_quots(t_elem **list, char *in, t_garbage **garbage)
{
	int	i;

	i = 0;
	while (in && in[i])
	{
		// if (in[i] && is_whitespace(in[i]))
		// 	ft_lstadd_back(list, ft_lstnew(ft_strdup(" ", garbage),
		// 			S_PACE, garbage));
		if (in[i] == '$' && in[i + 1] == '$')
			ft_lstadd_back(list, ft_lstnew(ft_strdup("$$", garbage),
					DOUBLE_DLR, garbage));
		else if (in[i] == '$')
			is_a_var(list, in, i, garbage);
		else if (in[i] &&  in[i] != '$')
			is_a_string(list, in, i, garbage);
		i += ft_strlen(ft_lstlast(*list)->content);
	}
	return (*list);
}

void	make_list(t_elem **head, char **str, t_garbage **grb)
{
	int		i;
	int		j;

	*head = NULL;
	i = ft_strlen2(str);
	j = 0;
	while (j < i)
	{
		ft_lstadd_back(head, ft_lstnew(str[j], WORD, grb));
		if (j != (i - 1))
			ft_lstadd_back(head, ft_lstnew(ft_strdup(" ", grb), S_PACE, grb));
		j++;
	}
}

void	add_to_list(t_elem **elem, t_elem *node, t_elem *head)
{
	t_elem	*current;
	t_elem	*prev;
	t_elem	*tmp;

	prev = NULL;
	tmp = NULL;
	current = *elem;
	while (current)
	{
		if (!ft_strcmp(current->content, node->content))
		{
			tmp = ft_lstlast(head);
			if (current->next)
				tmp->next = current->next;
			if (prev)
				prev->next = head;
			else
				*elem = head;
		}
		else
			prev = current;
		current = current->next;
	}
}

void	ft_split_var(t_elem **elem, t_elem *node, t_garbage **garbage)
{
	t_elem	*new_list;
	t_elem	*prev;
	t_elem	*current;
	char	**str;

	str = NULL;
	if (ft_strchr(node->content, '\t'))
		str = ft_split(node->content, '\t', garbage);
	else if (ft_strchr(node->content, ' '))
		str = ft_split(node->content, ' ', garbage);
	new_list = NULL;
	prev = NULL;
	current = *elem;
	make_list(&new_list, str, garbage);
	add_to_list(elem, node, new_list);
}
