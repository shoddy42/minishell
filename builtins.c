#include "minishell.h"
#include <stdio.h>
#include <unistd.h>

int    check_builtin(t_command    *cmd)
{
    if(ft_strcmp(cmd->options[0], "cd") == 0)
        ms_cd(cmd);
    else if(ft_strcmp(cmd->options[0], "echo") == 0)
        ms_echo(cmd);
    else if(ft_strcmp(cmd->options[0], "env") == 0)
        ms_env(cmd);
    else if(ft_strcmp(cmd->options[0], "pwd") == 0)
        ms_pwd(cmd);
    else if(ft_strcmp(cmd->options[0], "cd") == 0)
        ms_export(cmd);
    else if(ft_strcmp(cmd->options[0], "unset") == 0)
        ms_exit(cmd);
    else if(ft_strcmp(cmd->options[0], "exit") == 0)
        ms_exit(cmd);
}
