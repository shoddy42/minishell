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

/**
 * @brief Loops through all tokens after parsing. 
 * 		  Will append neighboring tokens of type COMMAND to eachother.
 * 
 * @param shell The shell.
 */
void	parse_append(t_minishell *shell)
{
	t_token	*tmp;

	tmp = shell->tokens;
	while(tmp && tmp->next)
	{
		if (tmp->type == COMMAND && tmp->next->type == COMMAND && tmp->data && tmp->next->data)
		{
			tmp->data = ft_strexpand(tmp->data, tmp->next->data);
			free_single_token(tmp->next);
		}
		else
			tmp = tmp->next;
	}
}

/**
 * @brief Checks infront of, and behind the pipe to check whether there is a syntax error.
 * 
 * @param token A token of which token->type == PIPE 
 * @param shell The shell.
 */
void	check_pipe(t_token *token, t_minishell *shell)
{
	t_token	*forward;
	t_token	*behind;

	forward = token->next;
	behind = token->prev;
	if (!forward || !behind)
	{
		shell->cancel_all_commands = true;
		return ;
	}
	while (forward && forward->next && forward->type != COMMAND && forward->type != PIPE)
		forward = forward->next;
	while (behind && behind->next && behind->type != COMMAND && forward->type != PIPE)
		behind = behind->next;
	// printf ("forward = (%s)[%s]\n", print_token_type(forward->type), forward->data);
	// printf ("behind = (%s)[%s]\n", print_token_type(behind->type), behind->data);
	if (forward->type != COMMAND || behind->type != COMMAND)
		shell->cancel_all_commands = true;
	shell->cancel_command = false;
	// printf ("cancel? [%i]\n", shell->cancel_all_commands);
}

// this function will have to be split into an expansion and a real parsing function

/**
 * @brief	Reads through all tokens and their types. Calls all relevant handle functions in order of:
 * 			Currently supported token->types: [LEFT] <, [RIGHT] >, ([QUOTE] ', [DQUOTE] "), [VARIABLE] $, [PIPE] |,
 * 			Quote and DQuote are handled in the same step.
 * 
 * @param shell The shell.
 */
void parse_token(t_minishell *shell)
{
	t_token		*token;

	token = shell->tokens;
	while (token && shell->cancel_all_commands == false)
	{
		// printf ("p (%s)[%s]\n", print_token_type(token->type), token->data);
		if (token->type == LEFT)
			token = handle_left(token, shell);
		if (token->type == RIGHT && shell->cancel_command == false)
			token = handle_right(token, shell);
		if (token->type == QUOTE || token->type == DQUOTE)
			token = handle_quote(token, token->type, shell);
		if (token->type == VARIABLE)
			expand_dong(token, shell);
		if (token->type == PIPE)
			check_pipe(token, shell);
		if (token->type == ERROR)
			shell->cancel_command = true;
		if (!token || !token->next)
			break;
		token = token->next;
	}
	parse_append(shell);
}

// function should be finished. unless we want to add semicolon support.
// incorperate this into parse_token, and change the function to check for syntax errors near |
int	count_pipes(t_minishell *shell)
{
	t_token	*tmp;

	tmp = shell->tokens;
	shell->pipe_count = 0;
	while (tmp)
	{
		if (tmp->type == PIPE)// || tmp->type == SEMICOLON)
			shell->pipe_count++;
		if (!tmp->next)
			break;
		tmp = tmp->next;
	}
	// printf ("pipe count real: [%i]\n", shell->pipe_count);
	return (0);
}

//not fully functional, but works for basic testing


//condense this.. last half of it should not be needed.
int	free_commands(t_minishell *shell)
{
	t_command	*tmp;
	int			i;

	tmp = shell->commands;
	if (!tmp)
		return (-12);
	while (tmp && tmp->next)
	{
		i = -1;
		if (tmp->command)
		{
			while (tmp->command[++i])
				free(tmp->command[i]);
			free (tmp->command);
		}
		tmp = tmp->next;
		if (tmp->prev)
			free(tmp->prev);
	}
	if (tmp)
	{
		i = -1;
		if (tmp->command)
		{
			while (tmp->command[++i])
				free(tmp->command[i]);
			free(tmp->command);
		}
		free(tmp);
	}
	shell->commands = NULL;
	return (0);
}

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
		exit (shell->last_return);
	}
	return (0);
}

//later: make more test cases and more todos :)
//later: make sure EVERY alloc is protected properly.
int	main(int ac, char **av, char **envp)
{
	t_minishell	*shell;

	shell = ft_calloc(1, sizeof(t_minishell));
	if (!shell)
		ms_error("FAILED TO ALLOCATE SHELL STRUCT, YOU HAVE LITERALLY 0 MEMORY LMAO", -1, true, NULL);
	shell->bin = opendir("./bin");
	printf ("open bins\n");
	if (!shell->bin)
		perror("no bins lmao");
	init_minishell(shell, envp);
	// exit (0);
	dash_c(shell, av);
	while (shell->exit == 0)
	{
		shell->hd_count = 0;
		shell->command = readline("minishell > ");
		if (shell->command == NULL) // todo: make it so we actually write exit with rl_replace_line somehow
		{
			// printf ("test\n");
			rl_replace_line("minishell> exit", 0);
			rl_on_new_line();
			rl_redisplay();
			exit (1);
		}
		if (shell->command[0] == 'q' && ft_strlen(shell->command) == 1) //temporary exits
			exit (0);
		if (ft_strcmp(shell->command, "exit") == 0)
			shell->exit = 1;
		ft_tokenize(shell, shell->command); //immune to error
		// print_tokens(shell);
		parse_token(shell); //if error, cut out everything
		// print_tokens(shell);
		count_pipes(shell);
		// printf ("how many?: [%i]\n", shell->hd_count);
		if (shell->cancel_all_commands == false)
		{
			if (make_commands(shell) == 0)
			{
				// print_commands(shell);
				execute_two_electric_boogaloo(shell);
			}
			printf("$? [%i]\n", shell->last_return); //print last cmd return
			// print_tokens(shell);
			// print_tokens_backwards(shell); //for testing whether prev is linked properly.

			//america AKA freeing all that needs freeing, resetting all that needs resetting.
			free_commands(shell);

			// reset signals after changing them in exe
			if (signal(SIGINT, sighandler) == SIG_ERR)
				exit (55);
		}
		else
			printf ("SYNTAXICAL ERROR\n"); //todo: make actual syntax error reporter, which will also work with <<<<< and >>>>>
		delete_heredocs(shell);
		free_tokens(shell);
		if (ft_strlen(shell->command) > 0)
			add_history(shell->command);
		if (shell->command)
			free(shell->command);
		shell->cancel_all_commands = false;
		shell->cancel_command = false;
		// if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		// 	exit (56);
	}
	return (0);
}
