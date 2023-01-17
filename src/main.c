/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 20:07:26 by wkonings      ########   odam.nl         */
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
static int	dash_c(t_minishell *shell, char **av, int ac)
{
	if (av[1] && ft_strcmp(av[1], "-c") == 0)
	{
		if (av[2])
			shell->command = av[2];
		else
			ms_error("NO COMMAND STR.", -9, true);
		tokenize(shell, shell->command);
		parse_token(shell);
		count_pipes(shell);
		make_commands(shell);
		execute_two_electric_boogaloo(shell);
		free_commands(shell);
		free_tokens(shell);
		delete_heredocs(shell);
		exit (g_status);
	}
	return (ac);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	*shell;

	shell = init_minishell(envp);
	dash_c(shell, av, ac);
	while (shell->exit == 0)
	{
		create_bin(shell);
		shell->hd_count = 0;
		shell->command = readline(shell->prompt);
		if (shell->prompt_type == 0)
			change_prompt(NULL, shell);
		if (shell->command == NULL)
			exit (0);
		shell->command_len = ft_strlen(shell->command);
		if (ft_strcmp(shell->command, "exit") == 0)
			shell->exit = 1;
		tokenize(shell, shell->command);
		parse_token(shell);
		count_pipes(shell);
		if (shell->cancel_all_commands == false)
			if (make_commands(shell) == 0)
				execute_two_electric_boogaloo(shell);
		ms_cleanup(shell);
	}
	return (0);
}
