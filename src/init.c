/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 09:24:40 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/01 17:33:42 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sighandler(int signum)
{
	if(signum == SIGINT) // ctrl + C
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}

	//WHEN ALL DONE CAN REMOVING MEN
	exit(1);
}

int	init_minishell(t_minishell *shell, char **envp)
{
	// signal handlers
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);

	// environment setup
	shell->envp = envp;
	shell->bin_dir = getcwd(NULL, 0);
	shell->bin_dir = ft_strexpand(shell->bin_dir, "/bin/");
	printf ("shell dir: [%s]\n", shell->bin_dir);
	// shell->last_return = 0;
	init_env(shell, envp);

	//todo: add things to initialize? kek

	return (0);
}