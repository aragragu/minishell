#include <libc.h>

typedef	struct c_cmd
{
	char			*cmd;
	char			**argc;
	struct c_cmd	*next;
}					t_cmd;



t_cmd	*ft_lstnew_cmd(char *str, char **gtr)
{
	t_cmd	*new_node;

	new_node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_node)
		return (NULL);
	new_node->cmd = str;
	new_node->argc = gtr;
	new_node->next = NULL;
	return (new_node);
}

void ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new)
{
    if (!lst || !new)
        return;
    
    if (*lst == NULL)
        *lst = new;
    else
    {
        t_cmd *ptr = *lst;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = new;
    }
}

void print_cmd(t_cmd *cmd) 
{
    if (!cmd)
        return ;
    while (cmd) 
    {
        if (cmd->cmd)
            printf("Command: %s\n", cmd->cmd);
        else 
            printf("Command: (null)\n");
        if (cmd->argc) 
        {
            printf("Arguments:\n");
            for (int i = 0; cmd->argc[i] != NULL; i++) 
                printf("  Arg[%d]: %s\n", i, cmd->argc[i]);
        } 
        else 
            printf("Arguments: (null)\n");
        cmd = cmd->next;
    }
}

int main()
{
    char *var[100] = {"export", "usdh", "uafg", NULL};
    char *varr[100] = {"export", "usdh", "uafg", NULL};
    char *varrr[100] = {"export", "usdh", "uafg", NULL};
    t_cmd *cmd = NULL;
    ft_lstadd_back_cmd(&cmd, ft_lstnew_cmd("export", var));
    ft_lstadd_back_cmd(&cmd, ft_lstnew_cmd("unset", varr));
    ft_lstadd_back_cmd(&cmd, ft_lstnew_cmd("set", varrr));
    // print_cmd(cmd);
}