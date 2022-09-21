/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/21 19:44:51 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_command_options(t_token	*token)
{
	char	**options;
	t_token	*tmp;
	int		i;

	tmp = token;
	i = 0;
	while (tmp && (tmp->type == COMMAND || tmp->type == VOID))
	{
		if (tmp->type == COMMAND)
			i++;
		tmp = tmp->next;
	}
	options = ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while (token && (token->type == COMMAND || token->type == VOID))
	{
		if (token->type == COMMAND)
		{
			options[i] = ft_strdup(token->data);
			i++;
		}
		token = token->next;
	}
	return (options);
}

void	expand_dong(t_token *token)
{
	char *tmp;

	tmp = getenv(token->data + 1);
	token->data = tmp;
}

t_token	*handle_quote_parse(t_token *token)
{
	t_token	*tmp;
	t_token	*ret;
	char	*str;

	tmp = token->next;
	str = ft_calloc(1, 1); // required for WSL at least otherwise we segmafault on ft_strexpand
	ret = ft_calloc(1, sizeof(t_token));
	printf("start quote handle\n");
	while (tmp && tmp->next)
	{
		printf("tmp??: %s\n", tmp->data);
		str = ft_strexpand(str, tmp->data);
		tmp = tmp->next;
		if (tmp->type == QUOTE)
		{
			if (tmp->next)
				tmp = tmp->next;
			break;
		}
	}
	token->data = ft_strdup(str);
	if (str)
		free(str);
	printf("expanded: %s\n", token->data);
	token->next = tmp;
	if (token->next->type == QUOTE)
		token->next = NULL;
	token->type = COMMAND;
	return (tmp);
}

t_token	*handle_quote_finalcope(t_token *token)
{
	t_token	*tmp;
	t_token	*ret;
	char	*str;

	tmp = token->next;
	str = ft_calloc(1, 1); // required for WSL at least otherwise we segmafault on ft_strexpand
	ret = ft_calloc(1, sizeof(t_token));
	printf("start quote handle\n");
	while (tmp && tmp->type != QUOTE && tmp->next)
	{
		// printf("tmp??: %s\n", tmp->data);
		str = ft_strexpand(str, tmp->data);
		tmp = tmp->next;
	}
	if (tmp->type == QUOTE)
		if (tmp->next)
			tmp = tmp->next;
	token->data = ft_strdup(str);
	if (str != NULL)
		free(str);
	free_next_tokens(token);
	printf("expanded: %s\n", token->data);
	// token->next = tmp;
	// if (token->next->type == QUOTE)
	// 	token->next = NULL;
	if (token)
		token->type = COMMAND;
	return (tmp);
}


void parse_token(t_minishell *shell)
{
	t_token		*token;
	t_command	*cmd;
	int			cmd_finished = 0;
	int			i;

	i = 0;
	token = shell->tokens;
	cmd = ft_calloc(1, sizeof(t_command));
	while (token)
	{
		// if (token->type == QUOTE)
		// 	token = handle_quote_finalcope(token);
		if (token->data[0] == '$')
			expand_dong(token);
		if (token->type == COMMAND && cmd->command == NULL)
			cmd->command = ft_strdup(token->data);
		if (cmd->command && cmd->options == NULL && token->type == COMMAND)
			cmd->options = get_command_options(token);
		token = token->next;
	}
	if (cmd->command)
		execute(cmd, shell);
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
	// shell->env = NULL;
	
	//todo: make function that gets env probably with getenv command instead of thru main.

	return (0);
}

int	main(int ac, char **av, char **env)
{
	char		*command;
	int			tmp_exit = 0;
	t_minishell	*shell;

	shell = ft_calloc(1, sizeof(t_minishell));
	init_minishell(shell);
	shell->envp = env;
	init_env(shell, env);
	while (tmp_exit == 0)
	{
		command = readline("> ");
		if (command[0] == 'q' && ft_strlen(command) == 1)
			tmp_exit = 1;
		ft_tokenize(shell, command);
		parse_token(shell);
		if (ft_strlen(command) > 0)
			add_history(command);
		if (command)
			free(command);
		print_tokens(shell);
		free_tokens(shell);
	}
	return (0);
}
