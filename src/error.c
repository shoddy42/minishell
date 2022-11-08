/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 05:59:42 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/08 23:04:15 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    cancel_commands(t_minishell *shell)
{
    // t_command   *cmd;

    // printf ("true?\n");
    shell->cancel_command_line = true;
    // printf ("truest?\n");

    // cmd = shell->commands;
    // while (cmd)
    // {
    //     cmd->executable = false;
    //     cmd = cmd->next;
    // }
}

//later: evaluate whether this is even needed at all.
void    ms_error(char *msg, int code, bool terminate, t_minishell *shell)
{
    errno = code;
    perror(msg);
    if (terminate == true)
        exit(code);
    else if (shell)
        cancel_commands(shell);
    //later: else, cancel current commandline.
}
