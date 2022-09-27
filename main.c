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

// t_token	*handle_quote_parse(t_token *token)
// {
// 	t_token	*tmp;
// 	t_token	*ret;
// 	char	*str;

// 	tmp = token->next;
// 	str = ft_calloc(1, 1); // required for WSL at least otherwise we segmafault on ft_strexpand
// 	ret = ft_calloc(1, sizeof(t_token));
// 	printf("start quote handle\n");
// 	while (tmp && tmp->next)
// 	{
// 		printf("tmp??: %s\n", tmp->data);
// 		str = ft_strexpand(str, tmp->data);
// 		tmp = tmp->next;
// 		if (tmp->type == QUOTE)
// 		{
// 			if (tmp->next)
// 				tmp = tmp->next;
// 			break;
// 		}
// 	}
// 	token->data = ft_strdup(str);
// 	if (str)
// 		free(str);
// 	printf("expanded: %s\n", token->data);
// 	token->next = tmp;
// 	if (token->next->type == QUOTE)
// 		token->next = NULL;
// 	token->type = COMMAND;
// 	return (tmp);
// }

void	free_tokens_til(t_token *start, t_token *end)
{
	t_token *tmp;

	while (start != end)
	{
		tmp = start;
		start = start->next;
		if (tmp->data)
			free(tmp->data);
		free(tmp);
	}
}

t_token	*handle_quote_finalcope(t_token *token)
{
	t_token	*tmp;
	char	*str;

	tmp = token->next;
	str = ft_calloc(1, 1); // required for WSL at least otherwise we segmafault on ft_strexpand
	printf("start quote handle\n");
	while (tmp && tmp->type != QUOTE && tmp->next)
	{
		str = ft_strexpand(str, tmp->data);
		tmp = tmp->next;
	}
	if (tmp->type == QUOTE)
	{
		printf ("current token: [%s]\n", tmp->data);
		if (tmp->next)
			tmp = tmp->next;
		else
		{
			token->next = NULL;
			// tmp = NULL;
			printf ("ELSE SCENARIO DUNNO LOL\n");
		}
		// token->next = NULL;
	}
	else
		printf ("WARNING: UNCLOSED QUOTE\n");
	free_tokens_til(token->next, tmp);
	token->data = str;
	token->next = tmp;
	// token->next = NULL;
		printf("next is quote\n");
	printf("expanded: %s\n", token->data);
	// if (token)
	// 	token->type = COMMAND;
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
		if (token->type == QUOTE)
			token = handle_quote_finalcope(token);
		if (!token)
			printf("WARNING SEGFAULT INCOMING XDDD\n");
		if (token->data[0] == '$')
			expand_dong(token);
		if (token->type == COMMAND && cmd->command == NULL)
			cmd->command = ft_strdup(token->data);
		if (cmd->command && cmd->options == NULL && token->type == COMMAND)
			cmd->options = get_command_options(token);
		// if (token->next) //maybe removing the if statingment men
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
		shell->command = readline("> ");
		if (shell->command[0] == 'q' && ft_strlen(shell->command) == 1)
			shell->exit = 1;
		ft_tokenize(shell, shell->command);
		parse_token(shell);
		if (ft_strlen(shell->command) > 0)
			add_history(shell->command);
		if (shell->command)
			free(shell->command);
		print_tokens(shell);
		free_tokens(shell);
	}
	return (0);
}
