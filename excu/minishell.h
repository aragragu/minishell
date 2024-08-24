/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:27:14 by ykasmi            #+#    #+#             */
/*   Updated: 2024/08/24 17:16:53 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libc.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct c_cmd
{
	char			*cmd;
	char			**argc;
	struct c_cmd	*next;
}					t_cmd;

typedef struct s_list
{
	char			*cmd;
	char			**arg;
	struct s_list	*next;
}					t_list;

typedef struct s_env
{
	char			*key;
	char			*val;
	int				flag;
	struct s_env	*next;
}					t_env;
typedef struct s_var
{
	t_list			*list;
	t_env			*env;
}					t_var;

t_env	*ft_lstnew(char *key, char *val);
int		ft_strlen(char *str);
t_env	*ft_lstlast(t_env *lst);
void	ft_lstadd_back(t_env **lst, t_env *new);
char	*ft_cat(char *str, int len, int flag);
void	norm_init_env(t_env **envr, char **env);
void	init_env(t_env **envr, char **env);
void	ft_env(t_env **env);
void	init_flag(t_env *env);
int		ft_strcmplist(char *s1, char *s2);
int		ft_lstsize(t_env *lst);
void	sort_env(t_env *env);
void	ft_export(t_var *var, int i, int error);
int		ft_strcmp(char *s1, const char *s2);
int		ft_isalpha(char c);
int		ft_digits(char c);
char	*ft_strchr(char *str, int c);
char	*ft_strjoin(char *s1, char *s2);
void	ft_echo(t_var *var);
void	ft_pwd(void);
void	ft_exit(t_var *var);
void	ft_cd(t_var *var);
void	ft_unset(t_var *var);
char	*ft_strdup(char *s1);
// void	ft_lstadd_back_cmd(t_list **lst, t_list *new);
// t_list	*ft_lstnew_cmd(char *str, char **gtr);
int		is_num(const char *str);
void	ft_putstr(char *str);
void	pwd_upd_old(t_env **env, char *key, char *val);
void	env_key_error(char **cmd, t_env **env, int i, char *msg);

#endif