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

//function simpler than i thought, probably merge with parse_apend. might not even need parse_append so im holding off
void	append_token(t_token *start, t_token *end)
{
	start->data = ft_strexpand(start->data, end->data);
	free_single_token(end);
}

void	parse_append(t_minishell *shell)
{
	t_token	*tmp;

	tmp = shell->tokens;
	while(tmp && tmp->next)
	{
		if (tmp->type == COMMAND && tmp->next->type == COMMAND)
		{
			// append_token(tmp, tmp->next); //replaced?
			tmp->data = ft_strexpand(tmp->data, tmp->next->data);
			free_single_token(tmp->next);
		}
		else
			tmp = tmp->next;
	}
}

t_token	*find_start_of_command(t_token *token, t_minishell *shell)
{
	t_token	*tmp;

	tmp = token;
	while (tmp && tmp->type != PIPE)
	{
		if (!tmp->prev)
			break;
		tmp = tmp->prev;
	}
	// printf ("start? (%s) [%s]\n", print_token_type(tmp->type), tmp->data);
	return (tmp);
}

t_token	*find_end_of_command(t_token *token, t_minishell *shell)
{
	t_token	*tmp;

	tmp = token;
	while (tmp && tmp->type != PIPE)
	{
		if (!tmp->next)
			break;
		tmp = tmp->next;
	}
	// printf ("end? (%s) [%s]\n", print_token_type(tmp->type), tmp->data);
	return (tmp);
}

t_token	*remove_command(t_token *token, t_minishell *shell)
{
	t_token	*start;
	t_token *end;

	start = find_start_of_command(token, shell);
	end = find_end_of_command(token, shell);
	free_tokens_til(start, end);
	return (end);
}

// this function will have to be split into an expansion and a real parsing function
void parse_token(t_minishell *shell)
{
	t_token		*token;
	int			i;

	i = 0;
	token = shell->tokens;
	while (token)
	{
		// printf("handling token [%s]\n", token->data);
		if (token->type == LEFT)
			token = handle_left(token, shell);
		if (token->type == RIGHT)
			token = handle_right(token, shell);
		if (token->type == QUOTE || token->type == DQUOTE)
			token = handle_quote(token, token->type, shell);
		if (token->type == VARIABLE)
			expand_dong(token, shell);
		if (token->type == ERROR)
			token = remove_command(token, shell);
		if (!token)
			break;
		token = token->next;
	}
	parse_append(shell);
}

// function should be finished. unless we want to add semicolon support.
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
		make_commands(shell);
		execute_two_electric_boogaloo(shell);
		exit (shell->last_return);
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

//later: make more test cases and more todos :)
//later: make sure EVERY alloc is protected properly.
int	main(int ac, char **av, char **envp)
{
	t_minishell	*shell;

	shell = ft_calloc(1, sizeof(t_minishell));
	if (!shell)
		ms_error("FAILED TO ALLOCATE SHELL STRUCT, YOU HAVE LITERALLY 0 MEMORY LMAO", -1, true, NULL);
	init_minishell(shell, envp);
	dash_c(shell, av);
	while (shell->exit == 0)
	{
		shell->hd_count = 0;
		shell->command = readline("minishell> ");
		// shell->cancel_command = false;
		if (shell->command == NULL) // todo: make it so we actually write exit with rl_replace_line somehow
		{
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
		if (make_commands(shell) == 0)
			execute_two_electric_boogaloo(shell);
		// print_commands(shell);
		if (ft_strlen(shell->command) > 0)
			add_history(shell->command);
		printf("$? [%i]\n", shell->last_return);
		// print_tokens(shell);
		// print_tokens_backwards(shell); //for testing whether prev is linked properly.

		//america
		free_commands(shell);
		free_tokens(shell);
		if (shell->command)
			free(shell->command);
		delete_heredocs(shell);
	}
	return (0);
}
