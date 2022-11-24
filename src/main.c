/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/24 23:23:46 by wkonings      ########   odam.nl         */
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
		if (shell->prompt_type == 0)
			change_prompt(NULL, shell);
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
