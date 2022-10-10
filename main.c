/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/22 18:01:37 by wkonings      ########   odam.nl         */
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

t_token	*handle_quote(t_token *token)
{
	t_token	*tmp;
	char	*str;

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


t_token	*handle_left(t_token *token, t_minishell *shell)
{
	t_token *tmp;

	tmp = token;
	if (tmp->next && tmp->next->type == LEFT)
	{
		tmp = tmp->next;
		tmp = heredoc(tmp, shell);
		if (tmp->type == HEREDOC_FILE)
	
	}
	return (token);
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
		print_tokens(shell);
		if (token->type == LEFT)
			token = handle_left(token, shell);
		if (token->type == QUOTE)
			token = handle_quote(token);
		if (!token)
			printf("WARNING SEGFAULT INCOMING xdd\n");
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

void	insert_prompt(t_minishell	*shell)
{
	shell->command = readline("> ");	
}

void	 sighandler(int signum)
{
	if(signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if(signum == SIGQUIT)
	{
		ft_putstr_fd("\b\b", 1);
	}

	//WHEN ALL DONE CAN REMOVING MEN
	exit(1);
}

int	init_minishell(t_minishell *shell)
{
	//todo: add things to initialize? kek

	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);

	// shell->env = NULL;
	
	//todo: make function that gets env probably with getenv command instead of thru main.

	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_minishell	*shell;

	shell = ft_calloc(1, sizeof(t_minishell));
	init_minishell(shell);
	shell->envp = env;
	init_env(shell, env);
	while (shell->exit == 0)
	{
		shell->command = readline("minishell> ");
		if (shell->command[0] == 'q' && ft_strlen(shell->command) == 1)
			shell->exit = 1;
		ft_tokenize(shell, shell->command);
		parse_token(shell);
		if (ft_strlen(shell->command) > 0)
			add_history(shell->command);
		if (shell->command)
			free(shell->command);
		// print_tokens(shell);
		free_tokens(shell);
	}
	return (0);
}
