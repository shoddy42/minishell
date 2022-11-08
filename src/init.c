/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 09:24:40 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/08 02:30:48 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//todo: make another sighandler for children, so they exit properly when signalled.
//todo:
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


//todo: replace SHELL=minishell and test it
//todo: Keep track of SHLVL, and increment it by 1 each time a shell is called.
int	init_minishell(t_minishell *shell, char **envp)
{
	// signal handlers
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);

	// environment setup
	// ms_replace_env("SHELL=minishell", shell);
	shell->envp = envp;
	shell->bin_dir = getcwd(NULL, 0);
	shell->bin_dir = ft_strexpand(shell->bin_dir, "/bin/");
	// shell->last_return = 0;
	init_env(shell, envp);

	return (0);
}