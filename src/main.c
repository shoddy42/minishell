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
	while (token && shell->cancel_command_line == FALSE)
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

int	count_pipes(t_minishell *shell)
{
	t_token	*tmp;

	tmp = shell->tokens;
	shell->pipe_count = 0;
	while (tmp)
	{
		if (tmp->type == PIPE)
			shell->pipe_count++;
		if (!tmp->next)
			break;
		tmp = tmp->next;
	}
	printf ("pipe count real: [%i]\n", shell->pipe_count);
	return (0);
}

int	dash_c(t_minishell *shell, char **av)
{
	if (av[1] && ft_strcmp(av[1], "-c") == 0)
	{
		if (av[2])
			shell->command = av[2];
		else
			ms_error("NO COMMAND STR.", -9, TRUE, shell);
		ft_tokenize(shell, shell->command);
		parse_token(shell);
		make_commands(shell);
		execute_two_electric_boogaloo(shell);
		exit (0);
	}
	return (0);
}

int	free_commands(t_minishell *shell)
{
	t_command	*tmp;
	int			i;

	tmp = shell->commands;
	if (!tmp)
		return (-12);
	while (tmp)
	{
		i = -1;
		while (tmp->command[++i])
			free(tmp->command[i]);
		if (!tmp->next)
			break;
		tmp = tmp->next;
		free(tmp->prev);
	}
	free(tmp);
	shell->commands = NULL;
	return (0);
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
	dash_c(shell, av);
	while (shell->exit == 0)
	{
		shell->command = readline("minishell> ");
		shell->cancel_command_line = FALSE;
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
		count_pipes(shell);
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
		free_commands(shell);
		free_tokens(shell);
	}
	return (0);
}
