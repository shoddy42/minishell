/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/14 01:40:35 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_token(t_minishell *shell)
{
	t_token	*token;
	t_command *cmd;
	int cmd_finished = 0;

	token = shell->tokens;
	cmd = ft_calloc(1, sizeof(t_command));
	while (token)
	{
		if (token->type == COMMAND && cmd->command == NULL)
		{
			cmd->command = ft_strdup(token->data);
			printf("cmd: %s\n", cmd->command);
		}
		token = token->next;
	}
}

void sighandler(int signum)
{
	printf("caught signal: %i\n", signum);
	//TODO: actual handling of specific signals.

	// remove the exit later
	exit(69);
}

int	init_minishell(t_minishell *shell)
{
	//todo: add things to initialize? kek

	signal(SIGINT, sighandler);
	
	//todo: make function that gets env probably with getenv command instead of thru main.

	return (0);
}

int	main(void)
{
	char		*command;
	int			tmp_exit = 0;
	t_minishell	shell;

	init_minishell(&shell);
	while (tmp_exit == 0)
	{
		command = readline("> ");
		if (command[0] == 'q' && ft_strlen(command) == 1)
			tmp_exit = 1;
		ft_tokenize(&shell, command);
		parse_token(&shell);
		if (ft_strlen(command) > 0)
			add_history(command);
		free(command);
		print_tokens(&shell);
		free_tokens(&shell);
	}
	return (0);
}
