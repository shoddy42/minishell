/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/24 01:11:56 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/23 02:59:58 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_commands(t_minishell *shell)
{
	t_command	*tmp;
	int			i;

	tmp = shell->cmd_head;
	if (!tmp)
		return ;
	while (tmp)
	{
		i = -1;
		if (tmp->command)
		{
			while (tmp->command[++i])
				free(tmp->command[i]);
			free (tmp->command);
		}
		if (!tmp->next)
			break ;
		tmp = tmp->next;
		if (tmp->prev)
			free(tmp->prev);
	}
	if (tmp)
		free(tmp);
	shell->cmd_head = NULL;
}

void	ms_cleanup(t_minishell *shell)
{
	if (shell->cancel_all_commands == false)
	{
		free_commands(shell);
		if (signal(SIGINT, sighandler) == SIG_ERR)
			printf ("error: signal system call failed. sigint (ctrl + c)\n");
		if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
			printf ("error: signal system call failed. sigquit (ctrl + \\)\n");
	}
	delete_heredocs(shell);
	free_tokens(shell);
	if (ft_strlen(shell->command) > 0)
		add_history(shell->command);
	if (shell->command)
		free(shell->command);
	shell->cancel_all_commands = false;
	shell->cancel_command = false;
}
