/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/13 18:24:49 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// need to actually write this function, dont know how to make it extendable yet.

// char	*find_variable(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i] && str[i] != '$')
// 		i++;
// 	// if (str[i] == '$')
// }

// unsure whether this is one is REALLY just this simple.
void	expand_dong(t_token *token)
{
	char *tmp;

	tmp = getenv(token->data + 1);
	free(token->data);
	token->type = COMMAND; //might need other types too?
	token->data = ft_strdup(tmp);
}

// todo: make function work with DQUOTE and its expansion of variables.
t_token	*handle_quote(t_token *token)
{
	t_token	*tmp;
	char	*str;
	int		type; // to differenciate between QUOTE and DQUOTE and whether or not to expand variables.

	tmp = token;
	if (!tmp || !tmp->next)
	{
		printf("ERROR: OUT OF TOKENS\n");
		return (token);
	}
	str = ft_calloc(1, 1); // required for WSL at least otherwise we segmafault on ft_strexpand
	token->type = COMMAND;
	while (tmp && tmp->next && tmp->type != QUOTE)
	{
		if (tmp->next->type != QUOTE)
			str = ft_strexpand(str, tmp->next->data);
		tmp = tmp->next;
	}
	token->data = str;
	if (tmp->type == QUOTE && tmp->next)
	{
			tmp = tmp->next;
			free_tokens_til(token->next, tmp);
			token->next = tmp;
	}
	else
	{
		if (tmp->type != QUOTE)
			printf ("WARNING: UNCLOSED QUOTE\n");
		free_tokens_til(token->next, tmp);
		free_single_token(tmp);
		token->next = NULL;
	}
	return (token);
}

char	**get_command_options(t_token	*token)
{
	char	**commands;
	t_token	*tmp;
	int		i;

	tmp = token;
	i = 0;
	while (tmp && !(tmp->type == PIPE))
	{
		if (tmp->type == COMMAND)
			i++;
		tmp = tmp->next;
	}
	commands = ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while (token && (token->type == COMMAND || token->type == VOID))
	{
		if (token->type == COMMAND)
		{
			commands[i] = ft_strdup(token->data);
			i++;
		}
		token = token->next;
	}
	return (commands);
}

int	make_command(t_minishell *shell)
{
	t_command	*cmd;
	t_token		*token;

	cmd = ft_calloc(1, sizeof(t_command));
	shell->commands = cmd;


	return (0);
}
		// move everything related to command execution to somewhere else.
		// if (token->type == COMMAND && cmd->command == NULL)
		// 	cmd->command = ft_strdup(token->data);
		// if (cmd->command && cmd->options == NULL && token->type == COMMAND)
		// 	cmd->options = get_command_options(token); //todo: free this later.

// this function will have to be split into an expansion and a real parsing function
void parse_token(t_minishell *shell)
{
	t_token		*token;
	int			i;

	i = 0;
	token = shell->tokens;
	// cmd = ft_calloc(1, sizeof(t_command));
	// cmd->outfile = STDOUT_FILENO;
	// cmd->infile = STDIN_FILENO;
	while (token)
	{
		// print_tokens(shell); //for testing purposes.
		if (token->type == LEFT)
			token = handle_left(token, shell);
		if (token->type == RIGHT)
			token = handle_right(token, shell);
		if (token->type == QUOTE)
			token = handle_quote(token);
		if (token->type == VARIABLE)
			expand_dong(token);
		token = token->next;
	}
	make_command(shell);
	// if (cmd->executable == 1)
	// 	execute(cmd, shell);
	// if (cmd->command)
		// free (cmd->command); // not sure when to free.
	// free (cmd); // not sure when to free.
}

// if the function is just 1 line, do we need it?
void	insert_prompt(t_minishell	*shell)
{
	shell->command = readline("> ");
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	*shell;

	shell = ft_calloc(1, sizeof(t_minishell));
	init_minishell(shell, envp);
	while (shell->exit == 0)
	{
		shell->command = readline("minishell> ");
		if (shell->command == NULL) // todo: make it so we actually write exit with rl_replace_line somehow
			exit (0);
		if (shell->command[0] == 'q' && ft_strlen(shell->command) == 1) //temporary exit
			shell->exit = 1;
		if (ft_strcmp(shell->command, "exit") == 0)
			shell->exit = 1;
		ft_tokenize(shell, shell->command);
		parse_token(shell);
		if (ft_strlen(shell->command) > 0)
			add_history(shell->command);
		if (shell->command)
			free(shell->command);
		print_tokens(shell);
		// print_tokens_backwards(shell);
		free_tokens(shell);
	}
	return (0);
}
