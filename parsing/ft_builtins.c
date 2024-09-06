/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 10:13:32 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/06 18:24:57 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_isalpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int ft_digits(char c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	return (0);
}

void ft_putstr(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

void ft_putstr_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (fd < 0)
		return;
	if (!s)
		return;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

char *ft_strjoinnn(char *s1, char *s2)
{
	size_t i;
	size_t j;
	char *ss;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	ss = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (ss == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		ss[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
		ss[j++] = s2[i++];
	}
	ss[j] = '\0';
	return (free(s1), ss);
}

int ft_lstsizeee(t_env *lst)
{
	t_env *tmp;
	int i;

	tmp = lst;
	i = 0;
	while (tmp && tmp->next)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}

t_env *ft_lstlasttt(t_env *lst)
{
	t_env *node;

	node = lst;
	if (!node)
		return (NULL);
	while (node->next)
		node = node->next;
	return (node);
}

void ft_lstadd_backkk(t_env **lst, t_env *new)
{
	t_env *last;

	if (!*lst)
	{
		*lst = new;
		return;
	}
	last = ft_lstlasttt(*lst);
	last->next = new;
}

void env_key_error(char **cmd, t_env **env, int i, char *msg)
{
	(void)env;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(cmd[i], 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

char *ft_strduppp(char *s1)
{
	size_t i;
	size_t len;
	char *s2;

	i = 0;
	len = ft_strlen(s1) + 1;
	s2 = (char *)malloc(len);
	if (!s2)
		return (NULL);
	while (s1[i] != '\0')
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

t_env *ft_lstnewww(char *key, char *val)
{
	t_env *node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = val;
	node->flag = 0;
	node->next = NULL;
	return (node);
}

void ft_cd(t_var *var)
{
	char cwd[PATH_MAX];
	char old_pwd[PATH_MAX];
	char *home;

	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		perror("getcwd");
		return;
	}
	if (var->list->argc[1] == NULL)
	{
		home = getenv("HOME");
		if (home == NULL)
		{
			printf("minishell: cd: HOME not set\n");
			return;
		}
		if (chdir(home) != 0)
		{
			printf("minishell: cd: %s: No such file or directory\n", home);
			return;
		}
	}
	else
	{
		if (chdir(var->list->argc[1]) != 0)
		{
			printf("cd: %s: No such file or directory\n", var->list->argc[1]);
			return;
		}
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		pwd_upd_old(&var->env, "OLDPWD", old_pwd);
		pwd_upd_old(&var->env, "PWD", cwd);
	}
	else
		perror("getcwd");
}

int check_n_option(const char *str)
{
	int i;

	i = 2;
	while (*str)
	{
		if (str[0] == '-' && str[1] == 'n')
		{
			while (str[i])
			{
				if (str[i] != 'n')
					return (0);
				i++;
			}
			return (1);
		}
		str++;
	}
	return (0);
}

void ft_echo(t_var *var)
{
	int newline;
	int i;

	i = 1;
	newline = 1;
	// if (var->list->argc)
	// {
	// 	printf("\n");
	// 	return;
	// }
	while (var->list->argc[i] && check_n_option(var->list->argc[i]))
	{
		newline = 0;
		i++;
	}
	while (var->list->argc[i])
	{
		ft_putstr(var->list->argc[i]);
		if (var->list->argc[i + 1])
			ft_putstr(" ");
		i++;
	}
	if (newline)
		ft_putstr("\n");
}

void norm_init_env(t_env **envr, char **env)
{
	char *key;
	char *val;
	int i;
	int j;

	i = 1;
	j = 0;
	key = NULL;
	val = NULL;
	while (env[i])
	{
		j = -1;
		while (env[i][++j])
		{
			if (env[i][j] == '=')
			{
				key = ft_cat(env[i], j, 0);
				val = ft_cat(env[i], j + 1, 1);
				ft_lstadd_backkk(envr, ft_lstnewww(key, val));
				break;
			}
		}
		i++;
	}
}

void init_env(t_env **envr, char **env)
{
	int i;
	char *key;
	char *val;

	i = -1;
	key = NULL;
	val = NULL;
	*envr = ft_lstnewww(ft_strduppp("PATH"), ft_strduppp("/Users/ykasmi/Desktop/Minishell")); // protection
	if (!env || !env[0])
	{
		ft_lstadd_backkk(envr, ft_lstnewww(ft_strduppp("SHLVL"), ft_strduppp("1")));
		ft_lstadd_backkk(envr, ft_lstnewww(ft_strduppp("_"), ft_strduppp("/usr/bin/env")));
		return;
	}
	while (env[0][++i])
	{
		if (env[0][i] == '=')
		{
			key = ft_cat(env[0], i, 0);
			val = ft_cat(env[0], i + 1, 1);
			break;
		}
	}
	*envr = ft_lstnewww(key, val); // protection
	norm_init_env(envr, env);
}

char *ft_cat(char *str, int len, int flag)
{
	int i;
	char *strn;

	if (flag)
		strn = malloc(ft_strlen(str) - len + 1); // protection
	else
		strn = malloc(len + 1); // protection
	i = 0;
	if (flag)
	{
		while (len < ft_strlen(str))
			strn[i++] = str[len++];
	}
	else
	{
		while (i < len)
		{
			strn[i] = str[i];
			i++;
		}
	}
	strn[i] = '\0';
	return (strn);
}

void ft_env(t_env **env)
{
	t_env *tmp;

	tmp = *env;
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

int is_num(const char *str)
{
	while (*str)
	{
		if (!ft_digits(*str) && *str != '-')
			return (0);
		str++;
	}
	return (1);
}

long	ft_atoi(char *str, t_var *var)
{
	int		i;
	int		n;
	long	result;

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
		if ((result * n > LONG_MAX && result * n < LONG_MIN))
		{
			printf("exit\nexit: %s: numeric arg required\n", var->list->argc[1]);
			exit(255);
		}
		i++;
	}
	return (result * n);
}

void ft_exit(t_var *var)
{
	long num;
	int ac = 1;

	ac = 0;
	while (var->list->argc[ac])
		ac++;
	if (ac < 3)
	{
		num = ft_atoi(var->list->argc[1], var);
		if (is_num(var->list->argc[1]))
		{
			printf("exit\n");
			exit(num);
		}
		else
		{
			printf("exit\nexit: %s: numeric arg required\n", var->list->argc[1]);
			exit(255);
		}
	}
	else if (ac > 2)
		printf("exit\nbash: exit: too many arguments\n");
}

int ft_strcmplist(char *s1, char *s2)
{
	int i;

	i = -1;
	while (s1[++i])
	{
		if (s1[i] < s2[i])
			return (1);
		if (s1[i] > s2[i])
			return (2);
	}
	return (0);
}

void init_flag(t_env *env)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		tmp->flag = 0;
		tmp = tmp->next;
	}
}

void sort_env(t_env **env)
{
	t_env *tmp;
	t_env *first;
	int i;
	int j;

	i = ft_lstsizeee(*env);
	j = 0;
	while (i >= j)
	{
		first = *env;
		tmp = *env;
		while (tmp)
		{
			if (tmp->flag == 0)
				first = tmp;
			tmp = tmp->next;
		}
		tmp = *env;
		while (tmp)
		{
			if (ft_strcmplist(first->key, tmp->key) == 2 && !tmp->flag)
				first = tmp;
			else if (!ft_strcmplist(first->key, tmp->key) &&
					 ft_strlen(tmp->key) < ft_strlen(first->key) && !tmp->flag)
				first = tmp;
			tmp = tmp->next;
		}
		if (first->value)
			printf("declare -x %s=\"%s\"\n", first->key, first->value);
		else
			printf("declare -x %s\n", first->key);
		first->flag = 1;
		j++;
	}
	init_flag(*env);
	return;
}

t_env *index_key(t_env *env, char *key)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(key, tmp->key))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void ft_export(t_var *var, int i, int error)
{
	int j;
	int k;
	int flag_plus;
	char *new_val;
	char *key;
	t_env *index;

	flag_plus = 0;
	if (!var->list->argc[1])
		sort_env(&var->env);
	else
	{
		while (var->list->argc[++i])
		{
			if (var->list->argc[i][0] && (var->list->argc[i][0] == '_' || ft_isalpha(var->list->argc[i][0])))
			{
				j = ft_strlen(var->list->argc[i]);
				if (ft_strchr(var->list->argc[i], '='))
				{
					j = -1;
					while (var->list->argc[i][++j])
					{
						if (var->list->argc[i][j] == '=')
							break;
					}
					if (var->list->argc[i][j - 1] == '+')
					{
						flag_plus = 1;
						j--;
					}
				}
				k = 0;
				while (++k < j)
				{
					if (!(var->list->argc[i][k] == '_' || ft_isalpha(var->list->argc[i][k]) || ft_digits(var->list->argc[i][k])))
					{
						error = 1;
						break;
					}
				}
			}
			else
				error = 1;
			if (!error)
			{
				if (ft_strchr(var->list->argc[i], '='))
				{
					if (flag_plus)
					{
						new_val = ft_cat(var->list->argc[i], j + 2, 1);
						key = ft_cat(var->list->argc[i], j, 0);
						index = index_key(var->env, key);
						if (index && var->list->argc[i][j] == '+' && var->list->argc[i][j + 1] == '=')
							index->value = ft_strjoinnn(index->value, new_val);
						else
							ft_lstadd_backkk(&var->env, ft_lstnewww(ft_cat(var->list->argc[i], j, 0), ft_cat(var->list->argc[i], j + 2, 1)));
					}
					else
					{
						new_val = ft_cat(var->list->argc[i], j + 1, 1);
						key = ft_cat(var->list->argc[i], j, 0);
						index = index_key(var->env, key);
						if (index)
						{
							free(index->value);
							index->value = new_val;
						}
						else
							ft_lstadd_backkk(&var->env, ft_lstnewww(ft_cat(var->list->argc[i], j, 0), ft_cat(var->list->argc[i], j + 1, 1)));
					}
				}
				else
				{
					key = ft_cat(var->list->argc[i], j, 0);
					index = index_key(var->env, key);
					if (!index)
						ft_lstadd_backkk(&var->env, ft_lstnewww(ft_cat(var->list->argc[i], j, 0), NULL));
				}
			}
			else
				env_key_error(var->list->argc, &var->env, i, "export");
			flag_plus = 0;
			error = 0;
		}
	}
}

void ft_pwd(void)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}

void pwd_upd_old(t_env **env, char *key, char *val)
{
	t_env *current;
	t_env *new_node;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strduppp(val);
			return;
		}
		current = current->next;
	}
	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node)
	{
		new_node->key = ft_strduppp(key);
		new_node->value = ft_strduppp(val);
		new_node->next = NULL;
		new_node->flag = 1;
		ft_lstadd_backkk(env, new_node);
	}
}

void ft_unset(t_var *var)
{
	t_env *prev;
	t_env *curr;
	int i;

	prev = NULL;
	curr = var->env;
	i = 0;
	while (var->list->argc[++i])
	{
		while (curr)
		{
			if (ft_strcmp(curr->key, var->list->argc[i]) == 0)
			{
				if (prev)
					prev->next = curr->next;
				else
					var->env = var->env->next;
				free(curr->key);
				free(curr->value);
				free(curr);
				return;
			}
			prev = curr;
			curr = curr->next;
		}
		prev = NULL;
		curr = var->env;
	}
}

int check_builtins(char *str)
{
	if (!str)
		return (-1);
	else if (!ft_strcmp(str, "echo") || !ft_strcmp(str, "cd") ||
			 !ft_strcmp(str, "exit") || !ft_strcmp(str, "pwd") ||
			 !ft_strcmp(str, "export") || !ft_strcmp(str, "unset"))
		return (1);
	return (0);
}

void ft_builtins(t_var *var, char *str, t_cmd **cmd)
{
	(void)cmd;
	if (!ft_strcmp(str, "echo"))
		ft_echo(var);
	else if (!ft_strcmp(str, "exit"))
		ft_exit(var);
	else if (!ft_strcmp(str, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(str, "cd"))
		ft_cd(var);
	else if (!ft_strcmp(str, "unset"))
		ft_unset(var);
	else if (!ft_strcmp(str, "export"))
		ft_export(var, 0, 0);
	else
		printf("command not found");
}

///////////////////////////////////////////////////////////////

void print_t_list(t_cmd *list)
{
	t_cmd *current = list;
	while (current)
	{
		printf("Command: %s\n", current->cmd);
		if (current->argc)
		{
			printf("Arguments: ");
			for (int i = 0; current->argc[i] != NULL; i++)
				printf("%s ", current->argc[i]);
			printf("\n");
		}
		current = current->next;
	}
}

// Function to print t_env (environment linked list)
void print_t_env(t_env *env)
{
	t_env *current = env;
	while (current)
	{
		printf("Key: %s, Value: %s, Flag: %d\n", current->key, current->value, current->flag);
		current = current->next;
	}
}

// Function to print t_var
void print_t_var(t_var *var)
{
	if (var->list)
	{
		printf("Printing t_list (Commands and Args):\n");
		print_t_list(var->list);
	}
	else
		printf("No commands in t_list.\n");

	if (var->env)
	{
		printf("Printing t_env (Environment Variables):\n");
		print_t_env(var->env);
	}
	else
		printf("No environment variables in t_env.\n");
}