/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/24 01:37:48 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_status;

/**
 * @brief		Executes a single commandline in minishell, then exits minishell.
 * 
 * @param shell The shell.
 * @param av	The arg variables of the call to ./minishell.
 * @returns 0 on success. 1 on failure (currently doesnt happen.)
 */
int	dash_c(t_minishell *shell, char **av)
{
	if (av[1] && ft_strcmp(av[1], "-c") == 0)
	{
		if (av[2])
			shell->command = av[2];
		else
			ms_error("NO COMMAND STR.", -9, true, shell);
		ft_tokenize(shell, shell->command);
		parse_token(shell);
		count_pipes(shell);
		make_commands(shell);
		execute_two_electric_boogaloo(shell);
		free_commands(shell);
		free_tokens(shell);
		delete_heredocs(shell);
		exit (g_status);
	}
	return (0);
}

//todo: make comand and prompt work.
bool	change_prompt(t_minishell *shell)
{
	char	*old;

	if (!shell->command)
		return (false);
	old = shell->prompt;
	if (ft_strcmp(shell->command, "prompt") == 0 || shell->prompt_type == 0)
		shell->prompt_type++;
	if (ft_strcmp(shell->command, "turtle") == 0 || shell->prompt_type == 1)
		shell->prompt = AT BT;
	if (ft_strcmp(shell->command, "hell") == 0 || shell->prompt_type == 2)
		shell->prompt = HELL1 HELL2;
	if (ft_strcmp(shell->command, "dragon") == 0 || shell->prompt_type == 3)
		shell->prompt = DRAGON1 DRAGON2;
	if (ft_strcmp(shell->command, "spooky") == 0 || shell->prompt_type == 4)
		shell->prompt = SPOOKY1 SPOOKY2;
	if (ft_strcmp(shell->command, "moon") == 0 || shell->prompt_type == 5)
		shell->prompt = MOON1 MOON2;
	if (shell->prompt_type > 5)
		shell->prompt_type = 0;
	if (ft_strcmp(old, shell->prompt) != 0 && \
		ft_strcmp(shell->command, "prompt") != 0)
		shell->prompt_type = -1;
	// if (ft_strcmp(old, shell->prompt) != 0)
	// 	return (true);	
	return (false);
}

//later: make more test cases and more todos :)
//later: make sure EVERY alloc is protected properly.
//todo: norme.
int	main(int ac, char **av, char **envp)
{
	t_minishell	*shell;

	ac = 0;
	shell = init_minishell(envp);
	dash_c(shell, av);
	while (shell->exit == 0)
	{
		create_bin(shell);
		shell->hd_count = 0;
		shell->command = readline(shell->prompt);
		change_prompt(shell);
		if (shell->command == NULL)
			close_stdin(shell);
		shell->command_len = ft_strlen(shell->command);
		if (ft_strcmp(shell->command, "exit") == 0)
			shell->exit = 1;
		ft_tokenize(shell, shell->command);
		parse_token(shell);
		count_pipes(shell);
		if (shell->cancel_all_commands == false)
			if (make_commands(shell) == 0)
				execute_two_electric_boogaloo(shell);
		ms_cleanup(shell);
	}
	return (0);
}
