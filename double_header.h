












void		read_input(char **env);
void		initialize_variables(t_var *var, char **env);
int			not_special(char c);
t_elem		*token_input(t_elem **list, char **in, t_garbage **garbage);
void		token_input_1(t_elem **list, char *input, int i, t_garbage **garbage);
void		token_input_2(t_elem **list, char *input, int i, t_garbage **garbage);






void    get_terminal_attr(struct termios *original_termios)
{
    tcgetattr(STDIN_FILENO, original_termios);
}

void    restore_terminal_attributes(struct termios *original_termios)
{
    tcsetattr(STDIN_FILENO, TCSANOW, original_termios);
}