/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 05:59:42 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/28 11:51:13 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    cancel_commands(t_minishell *shell)
{
    // t_command   *cmd;

    // printf ("true?\n");
    shell->cancel_command_line = TRUE;
    // printf ("truest?\n");

    // cmd = shell->commands;
    // while (cmd)
    // {
    //     cmd->executable = FALSE;
    //     cmd = cmd->next;
    // }
}

void    ms_error(char *msg, int code, t_bool terminate, t_minishell *shell)
{
    perror(msg);
    if (terminate == TRUE)
        exit(code);
    else if (shell)
        cancel_commands(shell);
    //todo: else, cancel current commandline.
}
