// /* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/19 17:04:56 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// this function will have to be split into an expansion and a real parsing function
void parse_token(t_minishell *shell)
{
	t_token		*token;
	int			i;

	i = 0;
	token = shell->tokens;
	while (token)
	{
		if (token->type == LEFT)
			token = handle_left(token, shell);
		if (token->type == RIGHT)
			token = handle_right(token, shell);
		if (token->type == QUOTE || token->type == DQUOTE)
			token = handle_quote(token, token->type, shell);
		if (token->type == VARIABLE)
			expand_dong(token, shell);
		token = token->next;
	}
}

//todo: make more test cases and more todos :)
//todo: make sure EVERY alloc is protected properly.
int	main(int ac, char **av, char **envp)
{
	t_minishell	*shell;

	shell = ft_calloc(1, sizeof(t_minishell));
	if (!shell)
		ms_error("FAILED TO ALLOCATE SHELL STRUCT, YOU HAVE LITERALLY 0 MEMORY LMAO", -1, TRUE, NULL);
	init_minishell(shell, envp);
	if (av[1] && ft_strcmp(av[1], "-c") == 0)
	{
		if (av[2])
			shell->command = av[2];
		ft_tokenize(shell, shell->command);
		parse_token(shell);
		execute_two_electric_boogaloo(shell);
		exit (0);
	}
	while (shell->exit == 0)
	{
		shell->command = readline("minishell> ");
		if (shell->command == NULL) // todo: make it so we actually write exit with rl_replace_line somehow
		{
			rl_replace_line("minishell> exit", 0);
			rl_on_new_line();
			rl_redisplay();
			exit (1);
		}
		if (shell->command[0] == 'q' && ft_strlen(shell->command) == 1) //temporary exits
			shell->exit = 1;
		if (ft_strcmp(shell->command, "exit") == 0)
			shell->exit = 1;
		ft_tokenize(shell, shell->command);
		parse_token(shell);
		if (shell->cancel_command_line == FALSE)
		{
			make_commands(shell);
		// print_commands(shell);
			execute_two_electric_boogaloo(shell);
		}
		if (ft_strlen(shell->command) > 0)
			add_history(shell->command);
		if (shell->command)
			free(shell->command);
		// print_tokens(shell);
		// print_tokens_backwards(shell); //for testing whether prev is linked properly.
		free_tokens(shell);
	}
	return (0);
}
