/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 11:44:30 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/22 15:20:18 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <errno.h>
# include <stdint.h>
# include <stdarg.h>
# include <limits.h>
# include <sys/wait.h>
# include <string.h>
# include <paths.h>
# include <libc.h>
# include <dirent.h>
# include <termios.h>
# include <stdbool.h>

typedef enum s_token
{
	WORD = 1,
	D_QOUTS,
	S_QOUTS,
	VAR,
	DOUBLE_DLR,
	TILDE,
	EXIT_STATUS,
	S_PACE,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	OPENING_PARENTHESIS,
	CLOSING_PARENTHESIS,
	AND,
}				t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				flag;
	int				flag2;
	struct s_env	*next;
}					t_env;

typedef struct s_elem
{
	char			*content;
	t_token			type;
	int				fd;
	int				fd_here;
	int				ignore[2];
	struct s_elem	*next;
}					t_elem;

typedef struct s_garbage
{
	void				*value;
	struct s_garbage	*next;
}					t_garbage;

typedef struct s_redir
{
	char			*value;
	t_token			type;
	int				fd;
	struct s_redir	*next;
}			t_redir;

typedef struct c_cmd
{
	char			*cmd;
	char			**argc;
	t_redir			*redirection;
	int				fd[2];
	struct c_cmd	*next;
}					t_cmd;

typedef struct s_herdoc
{
	char		*line;
	char		*buffer;
	t_elem		*current;
	char		*tmp;
	char		*file_name;
	int			fd;
	int			cfd;
	int			flag;
}				t_herdoc;

typedef struct s_var
{
	t_cmd			*list;
	t_env			*env;
	t_garbage		*garbage;
	t_garbage		*garb;
	t_elem			*linked_list;
	t_env			*index;
	t_cmd			*list2;
	pid_t			pid[1000];
	int				flag;
	void			*ptr;
	char			*path;
	int				exit_num;
	int				j;
	int				k;
	int				error;
	int				flag_plus;
	char			*key;
	char			*new_val;
	char			**envp;
	bool			if_valid;
}					t_var;

int	g_sig;
char		*ft_itoa(int nb, t_garbage **garbage);
char		**ft_split(char const *s, char c, t_garbage **garbage);
void		ft_sign(char c, int *sign);
t_env		*ft_lstnew2(char *key, char *value, t_garbage **garbage);
t_garbage	*ft_lstnew_garbage(void *content);
t_elem		*ft_lstnew(void *content, t_token type, t_garbage **garbage);
t_elem		*ft_lstlast(t_elem *lst);
void		token_input(char **in, t_var *var);
t_env		*ft_lstlast2(t_env *lst);
void		fill_env(t_env **env, char **str, t_garbage **garbage);
void		ft_lstadd_back_garbage(t_garbage **lst, t_garbage *new);
void		ft_lstadd_back(t_elem **lst, t_elem *new);
int			ft_lstsize(t_elem *lst);
void		read_input(char **env);
int			sysntax_error_checker(t_garbage **garbage, char **str,
				t_elem **list);
int			is_whitespace(char c);
int			has_unclosed_quots(char *str);
void		is_a_word(t_elem **list, char *input, int index,
				t_garbage **garbage);
void		is_a_quot(t_elem **list, char *input, int index,
				t_garbage **garbage);
void		print_list(t_elem **list);
void		is_a_squot(t_elem **list, char *input, int index,
				t_garbage **garbage);
void		ft_free(char **tab);
int			ft_strlen(char *str);
char		*ft_substr(char *s, int start, int len, t_garbage **garbage);
int			ft_strlcpy(char *dest, char *src, int size);
void		*ft_calloc(int count, int size, t_garbage **garbage);
char		*ft_strdup(char *s1, t_garbage **garbage);
void		ft_bzero(void *s, int n);
void		is_a_var(t_elem **list, char *input, int index,
				t_garbage **garbage);
int			is_alphanumeric(char c);
char		*ft_strtrim(char *s1, char *set, t_garbage **garbage);
char		*ft_strchr(char *s, int c);
void		my_perror(t_garbage **garbage, t_elem **list, char *str);
int			pipe_error(t_elem **head);
int			has_invalid_redirection_in(t_elem **list);
int			has_invalid_redirection_out(t_elem **list);
int			has_invalid_heredoc(t_elem **list);
int			has_invalid_append(t_elem **list);
t_elem		*token_quots(t_elem **list, char *in, t_garbage **garbage);
int			has_logical_operators(char *str);
void		edit_list(t_elem *list, t_garbage **garbage);
void		expand_var_list(t_elem **list, t_var container,
				t_garbage **garbage);
void		ft_lstadd_back2(t_env **lst, t_env *new);
void		print_env_list(t_env *head);
int			ft_strcmp(char *s1, char *s2);
void		expand_var(t_elem **list, t_elem *node, t_env **env,
				t_garbage **garbage);
void		free_garbage(t_garbage **garbage);
void		expand_d_qouts(t_env **env, char **ptr, t_garbage **garbage);
void		expand_d_qouts_2(t_env **env, char **ptr, t_garbage **garbage);
char		*ft_strjoin(char *s1, char *s2, t_garbage **garbage);
int			not_special(char c);
void		is_a_string(t_elem **list, char *input, int index,
				t_garbage **garbage);
void		handle_redirection(t_elem **list);
void		redirection_out_list(t_elem **list);
void		redirection_in_list(t_elem **list);
void		herdoc_list(t_elem **list, t_env **env, t_garbage **garbage);
void		open_herdoc(t_elem **list, t_env **env, t_garbage **garbage);
void		append_list(t_elem **list);
void		starting_red(t_elem **list);
t_redir		*ft_lstnew_redi(char *value, t_token type,	int fd,
				t_garbage **garbage);
void		ft_lstadd_back_redi(t_redir **lst, t_redir *new);
void		import_data(t_cmd **cmd, t_elem **list, t_garbage **garbage);
void		fill_cmd1(t_cmd **cmd, t_elem **list, t_garbage **garbage);
t_cmd		*ft_lstnew_cmd(void);
void		ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new);
void		print_cmd(t_cmd *cmd);
void		rename_token(t_elem **list);
void		fill_cmd2(t_cmd **cmd, t_elem **list, t_garbage **garbage);
int			word_count(t_elem *list);
t_elem		*fill_argc(t_cmd **cmd, t_elem **list, t_garbage **garbage);
void		expand_herdoc(char **str, t_env **env, t_garbage **garbage);
void		concatination(t_elem **list, t_garbage **garbage);
int			is_special_character(char c);
int			ft_strlen2(char **str);
void		ft_split_var(t_elem **elem, t_elem *node, t_garbage **garbage);
int			has_invalid_logical_operator1(t_elem **list);	
int			has_invalid_logical_operator2(t_elem **list);
void		signal_handler(int sig); //
void		token_input_1(t_elem **list, char *input, int i, t_var *var);
void		token_input_2(t_elem **list, char *input, int i,
				t_garbage **garbage);
void		initialize_variables(t_var *var, char **env);
int			fill_linked_list(char *input, t_var *var);
int			check_fd_her(t_elem **elem);
void		handle_whitespace(char *input, int *i, t_elem **list,
				t_garbage **garbage);
int			handle_special_characters(char *input, int *i, t_var *var);
int			check_unclosed_quotes(t_garbage **garbage, char **str,
				t_elem **list);
int			check_logical_operators(t_garbage **garbage, t_elem **list);
int			check_pipe_errors(t_garbage **garbage, t_elem **list);
int			check_redirection_errors(t_garbage **garbage, t_elem **list);
int			check_append_and_heredoc(t_garbage **garbage, t_elem **list);
int			fill_and_check(char *input, t_var *var);
int			list_handler(t_var *var);
void		s_redir_in(t_elem **list);
void		s_redir_out(t_elem **list);
void		s_append(t_elem **list);
void		s_herdoc(t_elem **list, t_env **env, t_garbage **garbage);
void		redirection_out_list2(t_elem **list);
void		redirection_in_list2(t_elem **list);
void		append_list2(t_elem **list);
void		initiaize_herdoc(t_herdoc **data, int *i, t_garbage **garbage,
				t_elem **list);
int			herdoc_loop(t_elem **list, t_herdoc *data, t_garbage **garbage,
				t_env **env);
int			check_herdoc_line(t_elem **list, t_herdoc *data,
				t_garbage **garbage);
int			write_herdoc_line(t_elem **list, t_herdoc *data,
				t_garbage **garbage, t_env **env);
t_herdoc	*ft_lstnew3(t_elem **list, char *buffer, t_garbage **garbage);
int			handle_herdoc_list(t_elem **token, t_garbage **garbage);
int			handle_herdoc_list2(t_elem **token, t_garbage **garbage);
int			handle_herdoc_list3(t_elem **token);
void		edit_all_list(t_elem **list, t_garbage **garbage);
void		edit_linked_list(t_var *data, t_elem *list);
void		remove_delemiter(t_elem **list, t_garbage **garbage);
void		ft_heredoc(t_var *data);
void		sigint_herdoc(void);
void		s_handler(int sig);
void		open_herdoc_file(t_herdoc *data);
int			ft_atoi2(char *str);
void		expand_var2(t_elem *node, char *gtr, t_garbage **grb, t_env *env);
int			count_word(t_elem **list);
int			count_word2(t_elem **list);
char		**allocate_str(int i, char **str, t_cmd *command, t_garbage **grb);
void		fill_command(t_elem *current, char **str, t_cmd *command,
				t_garbage **grb);
t_elem		*fill_command_pipe(t_elem *current, char **str, t_cmd *command,
				t_garbage **grb);
t_elem		*fill_command_pipe2(t_elem *current, char **str, t_cmd *command,
				t_garbage **grb);
void		concatinate(t_elem **current, t_elem **new_list, t_garbage **grb);
void		process_nodes(t_elem **tmp, int *flag, char **buff, t_var *data);
void		handle_heredoc(t_elem *list, t_var *data);
void		expand_var_quots(t_elem **elem, t_elem *node, t_env **env,
				t_garbage **grb);
void		expand_var_quots2(t_elem *node, char *gtr, t_garbage **grb,
				t_env *env);
t_redir		*ft_lstlast_redi(t_redir *lst);

void		ft_echo(t_var *var);
void		ft_export(t_var *var, int i, int error);
void		ft_pwd(void);
void		ft_exit(t_var *var);
void		ft_cd(t_var *var);
void		ft_unset(t_var *var, int i);
void		sort_env(t_env **env);
void		ft_env(t_var *var);
int			ft_isalpha(char c);
int			ft_digits(char c);
char		*ft_cat(char *str, int len, int flag);
void		pwd_upd_old(t_var *var, char *key, char *val);
int			check_builtins(char *str);
char		*ft_strduppp(char *s1);
void		ft_builtins(t_var *var, char *str, t_cmd **cmd);
void		ft_lstadd_backkk(t_env **lst, t_env *new);
t_env		*ft_lstlasttt(t_env *lst);
int			ft_lstsizeee(t_env *lst);
t_env		*ft_lstnewww(char *key, char *val);
int			ft_isalpha(char c);
int			ft_digits(char c);
void		ft_putstr(char *str);
char		*ft_strjoinnn(char *s1, char *s2);
void		env_key_error(char **cmd, t_env **env, int i, char *msg);
int			count_env(t_env *envv);
char		*build_path(char *dir, char *filename);
char		*excu_in_path(char *filename, t_var *var);
void		ft_exc(t_var *var);
char		*ft_strchrr(char *str, int c);
char		*check_valid_path(char *filename, t_var *var);
char		*ft_getenv(t_env *env, char *key);
char		*ft_strcat(char *dest, char *src);
char		*ft_strcpy(char *dest, char *src);
void		execute_pipe(int num_cmds, t_var *var, int i, int prev_fd);
void		store_env(t_env *envv, char ***env, int i, int len);
void		ft_exc2(t_var *var);
int			contains_red(t_var *var);
void		handle_redirection2(t_var *var);
int			calculate_cmd(t_var *var);
void		ft_fprintf(int fd, const char *format, ...);
int			ft_putnbr_fd(int n, int fd);
int			ft_putnbr_unsd_fd(unsigned int n, int fd);
int			ft_putnbr_hexa_fd(unsigned long n, int a, int fd);
void		execution(t_var *var);
int			ft_putstr_fd(char *s, int fd);
int			ft_putchar_fd(char c, int fd);
void		error_function(t_var *var);
void		error_fork(pid_t pid);
void		waitpid_func(t_var *var, struct termios *term);
void		red_herd_appen(t_redir *redir, int fd, t_var *var);
void		red_out_in(t_redir *redir, int fd);
int			contains_red(t_var *var);
void		norm_excu_pipe3(t_var **var);
int			check_builtins(char *str);
void		ft_builtins(t_var *var, char *str, t_cmd **cmd);
int			count_env(t_env *envv);
char		*norm_excu_in_path(char *filename);
int			is_num(const char *str);
void		update_exit_status(int status);
char		*search_in_path(char *start, char *filename, t_var *var);
void		error_function_expo(t_var *var, int i);
void		ex2_norm(t_var *var, int i, int *error);
void		ex3_norm(t_var *var, int i);
void		ex4_norm(t_var *var, int i);
void		ex5_norm(t_var *var, int i);
t_env		*index_key(t_env *env, char *key);
void		ex1_norm(t_var *var, int i, int *error);
long		ft_atoi(char *str, t_var *var);
char		*ft_itoa1(int n);
void		swap_nodes(t_env **first, t_env **tmp);
void		print_export(t_env *first);
int			ft_strcmplist(char *s1, char *s2);
int			g_es(int stat, int flag);
void		signal_hand_sig_qui(int sig);
void		ft_strduppp2(char **s1);

#endif