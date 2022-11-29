/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/24 01:11:56 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/25 16:54:13 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// todo: make it so we actually write exit with rl_replace_line somehow
// void	close_stdin(t_minishell *shell)
// {
// 	// rl_replace_line("minishell> exit", 0);
// 	// rl_on_new_line();
// 	// rl_redisplay();
// 	exit (1);
// }

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
			exit (55);
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