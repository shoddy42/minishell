/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 09:24:40 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/18 20:24:40 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//todo: make another sighandler for children, so they exit properly when signalled.

void	child_sig(int signum)
{
	// printf ("KID SIGNAL HANDLED\n");
	if (signum == SIGINT)
		rl_redisplay();
}

void	sighandler(int signum)
{
	// printf ("PARENT SIGNAL: ");
	if (signum == SIGINT) // ctrl + C
	{
		// printf ("SIG INT\n");
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		// rl_redisplay();
		// write(1, "\n", 1);
	}
	// exit(1); //later: remove
}

void	increase_shlvl(t_minishell *shell)
{
	char	*shlvl;
	char	*num;

	num = ft_itoa(1 + ft_atoi(ms_getenv("SHLVL", shell)));
	shlvl = ft_strjoin("SHLVL=", num);
	ms_replace_env(shlvl, shell);
	free(shlvl);
	free(num);
}

void	init_env(t_minishell *shell, char **envp)
{
	int		i;
	t_env	*tmp;

	i = 0;
	shell->env = new_env(envp[i]);
	tmp = shell->env;
	while (envp[++i])
	{
		tmp->next = new_env(envp[i]);
		tmp = tmp->next;
	}
	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0 && envp[i + 1])
		i++;
	if (ft_strncmp(envp[i], "PATH=", 5) != 0)
		exit (1);
	shell->path = ft_split(envp[i] + 6, ':');
}

int	init_minishell(t_minishell *shell, char **envp)
{
	// signal handlers
	if (signal(SIGINT, sighandler) == SIG_ERR)
		exit (55);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		exit (56);
	// environment setup
	shell->cancel_all_commands = false;
	shell->last_return = 0;
	shell->envp = envp;
	shell->bin_dir = getcwd(NULL, 0);
	shell->bin_dir = ft_strexpand(shell->bin_dir, "/bin/");
	// shell->last_return = 0;
	init_env(shell, envp);
	ms_replace_env("SHELL=minishell", shell);
	increase_shlvl(shell);
	return (0);
}
