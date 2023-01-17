/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/24 01:05:49 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 20:20:25 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child_sig(int signum)
{
	extern int	g_status;

	if (signum == SIGQUIT)
		g_status = 131;
	if (signum == SIGINT)
	{
		g_status = 130;
		write(1, "\n", 1);
		rl_redisplay();
	}
}

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	heredoc_sig(int signum)
{
	if (signum == SIGINT)
		exit (1);
}
