/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 11:44:30 by aragragu          #+#    #+#             */
/*   Updated: 2024/09/16 13:45:55 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
// #include <libc.h>
#include <limits.h>
#include <sys/wait.h>

#define RED     "\x1b[31m"
#define GREEN   "\e[1;32m"
#define BLUE    "\e[1;38;5;87m"
#define RESET   "\x1b[0m"

#undef SPACE //remove if u are using macos
#undef PIPE //remove if u are using macos
#undef REDIR_IN //remove if u are using macos
#undef REDIR_OUT //remove if u are using macos
#undef HEREDOC //remove if u are using macos
#undef APPEND //remove if u are using macos

#define MAX_CMD_LEN 100
#define MAX_ARGS 10
#define MAX_CMDS 10

typedef enum    s_token
{
	WORD = 1,				// STRING
	D_QOUTS,        		// " "
	S_QOUTS,        		// ' '
	VAR,            		// $ASJASD
	DOUBLE_DLR,				// $$
	SPACE,					// "_"
	NULL_TOKEN,
	PIPE,           		// |
	REDIR_IN,       		// <
	REDIR_OUT,      		// >
	HEREDOC,        		// << 
	APPEND,         		// >>
	OPENING_PARENTHESIS,	// '('
	CLOSING_PARENTHESIS,	// ')'
	HASH					// #
}                t_token;

typedef struct   s_env
{
	char            *key;
	char			*value;
	int				flag;
	struct s_env	*next;
}                   t_env;


typedef struct   s_elem
{
	char            *content;
	t_token         type;
	struct s_elem   *next;
}                   t_elem;

typedef struct   s_garbage
{
	void            	*value;
	struct s_garbage	*next;
}                   t_garbage;

typedef struct s_redir
{
	char			*value;
	t_token			type;
	int 			fd;	
	struct	s_redir *next;
}			t_redir;

typedef	struct c_cmd
{
	char			*cmd;
	char			**argc;
	t_redir			*redirection;
	struct c_cmd	*next;
}					t_cmd;

////////////////////////////////
typedef struct s_var
{
	t_cmd			*list;
	t_env			*env;
}					t_var;
///////////////////////////////////////









char		*ft_itoa(int nb);
char		**ft_split(char const *s, char c, t_garbage **garbage);
void		ft_sign(char c, int *sign);
t_garbage	*ft_lstnew_garbage(void *content);
t_elem		*ft_lstnew(void *content, t_token type, t_garbage **garbage);
t_elem		*ft_lstlast(t_elem *lst);
t_elem		*token_input(t_elem **list, char **input, t_garbage **garbage);
t_elem		*token_quots(t_elem **list, char *in, t_garbage **garbage);
t_env		*ft_lstlast2(t_env *lst);
void    	fill_env(t_env **env, char **str, t_garbage **garbage);
t_env		*ft_lstnew2(char *key, char *value, t_garbage **garbage);
void 		ft_lstadd_back_garbage(t_garbage **lst, t_garbage *new);
void		ft_lstadd_back(t_elem **lst, t_elem *new);
int			ft_lstsize(t_elem *lst);
void		read_input(char **env);
int			sysntax_error_checker(t_garbage **garbage, char *str, t_elem **list);
int			is_withespace(char c);
int			has_unclosed_quots(char *str);
void		is_a_word(t_elem **list, char *input, int index, t_garbage **garbage);
void		is_a_quot(t_elem **list, char *input, int index, t_garbage **garbage);
void		print_list(t_elem **list);
void		is_a_squot(t_elem **list, char *input, int index, t_garbage **garbage);
// void		free_list(t_elem **list);
int			ft_strlen(char *str);
char		*ft_substr(char *s, int start, int len, t_garbage **garbage);
int			ft_strlcpy(char *dest, char *src, int size);
void		*ft_calloc(int count, int size, t_garbage **garbage);
char		*ft_strdup(char *s1, t_garbage **garbage);
void		ft_bzero(void *s, int n);
void		is_a_var(t_elem **list, char *input, int index, t_garbage **garbage);
int			is_alphanumeric(char c);
char		*ft_strtrim(char *s1, char *set, t_garbage **garbage);
char		*ft_strchr(char *s, int c);
void    	my_perror(t_garbage **garbage, t_elem **list, char *str);
int			pipe_error(t_elem **head); 
int     	has_invalid_redirection_in(t_elem **list);
int			has_invalid_redirection_out(t_elem **list);
int			has_invalid_heredoc(t_elem **list);
int 		has_invalid_append(t_elem **list);
int     	has_logical_operators(char *str);
void		edit_list(t_elem *list, t_garbage **garbage);
void		expand_var_list(t_elem **list, t_env **env, t_garbage **garbage);
void		ft_lstadd_back2(t_env **lst, t_env *new);
void		print_env_list(t_env *head);
int			ft_strcmp(char *s1, char *s2);
void    	expand_var(t_elem **list ,t_elem *node, t_env **env, t_garbage **garbage);
void    	free_garbage(t_garbage **garbage);
void	    expand_d_qouts(t_env **env, char **ptr, t_garbage **garbage);
void		expand_d_qouts_2(t_env **env, char **ptr, t_garbage **garbage);
char		*ft_strjoin(char *s1, char *s2, t_garbage **garbage);
int			not_special(char c);
void		is_a_string(t_elem **list, char *input, int index, t_garbage **garbage);
void		handle_redirection(t_elem **list, t_env **env,t_garbage **garbage);
void		redirection_out_list(t_elem **list);
void		redirection_in_list(t_elem **list);
void		herdoc_list(t_elem **list, t_env **env,t_garbage **garbage);
void		open_herdoc(t_elem **list, t_env **env,t_garbage **garbage, int flag);
void		append_list(t_elem **list);
void		starting_red(t_elem **list, t_env **env,t_garbage **garbage);
t_redir		*ft_lstnew_redi(char *value, t_token type, t_garbage **garbage);
void 		ft_lstadd_back_redi(t_redir **lst, t_redir *new);
void        import_data(t_cmd **cmd, t_elem **list, t_garbage **garbage);
void        fill_cmd1(t_cmd  **cmd, t_elem **list, t_garbage **garbage);
t_cmd		*ft_lstnew_cmd(void);
void 		ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new);
void 		print_cmd(t_cmd *cmd);
void    	rename_token(t_elem **list);
void    	fill_cmd2(t_cmd  **cmd, t_elem **list, t_garbage **garbage);
int			word_count(t_elem *list);
t_elem		*fill_argc(t_cmd  **cmd, t_elem **list, t_garbage **garbage);
void		expand_herdoc(char **str, t_env **env, t_garbage **garbage);
void		concatination(t_elem **list, t_garbage **garbage);
int			is_special_character(char c);
int ft_strlen2(char **str);
void    ft_split_var(t_elem **elem, t_elem *node, t_garbage **garbage);

//////////////////////////////////////////////////////////////////////////////

void		ft_echo(t_var *var);
void		ft_export(t_var *var, int i, int error);
void		ft_pwd(void);
void		ft_exit(t_var *var);
void		ft_cd(t_var *var);
void		ft_unset(t_var *var);
void		sort_env(t_env **env);
void		ft_env(t_env **env);
int			ft_isalpha(char c);
int			ft_digits(char c);
char		*ft_cat(char *str, int len, int flag);
void		pwd_upd_old(t_env **env, char *key, char *val);
void		ft_putstr_fd(char *s, int fd);
int			check_builtins(char *str);
void		init_env(t_env **envr, char **env);
char		*ft_strduppp(char *s1);
void		ft_builtins(t_var *var, char *str, t_cmd **cmd);
void		ft_lstadd_backkk(t_env **lst, t_env *new);
t_env		*ft_lstlasttt(t_env *lst);
int			ft_lstsizeee(t_env *lst);
t_env		*ft_lstnewww(char *key, char *val);
int			ft_isalpha(char c);
int			ft_digits(char c);
void		ft_putstr(char *str);
void		ft_putstr_fd(char *s, int fd);
char		*ft_strjoinnn(char *s1, char *s2);
void		env_key_error(char **cmd, t_env **env, int i, char *msg);
int			count_env(t_env *envv);
char		*build_path(char *dir, char *filename);
char		*excu_in_path(char *filename, t_var *var);
void		ft_exc(t_var *var);
char		*ft_strchrr(char *str, int c);
int			check_valid_path(char *filename, t_var *var);
char		*ft_getenv(t_env *env, char *key);
char		*ft_strcat(char *dest, const char *src);
char		*ft_strcpy(char *dest, const char *src);
void execute_pipeline(t_var *var, char **envp);
void execute_command(t_var *var, int input_fd, int output_fd, char **envp);
void	store_env(t_env *envv, char ***env);
void	ft_exc2(t_var *var);

#endif