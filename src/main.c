/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/12 21:00:45 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

// unsure whether this is one is REALLY just this simple.
void	expand_dong(t_token *token)
{
	char *tmp;

	tmp = getenv(token->data + 1);
	token->data = tmp;
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

t_token	*handle_left(t_token *token, t_minishell *shell)
{
	t_token *tmp;

	tmp = token;
	if (tmp->next && tmp->next->type == LEFT)
	{
		tmp = tmp->next;
		tmp = heredoc(tmp, shell);
		if (tmp->type == HEREDOC_FILE)
			printf("heredoc time\n");
	}
	return (token);
}

/*
	todo: add check for >>> and possibly other forms of wrng input.
	todo: split function so it's norme
*/

t_token	*handle_right(t_token *token, t_minishell *shell, t_command *cmd)
{
	t_token 	*tmp;
	int			append;

	append = 0;
	tmp = NULL;
	printf("\nentered handle_right on token [%s]\n", token->data);
	if (token->next)
		tmp = token->next;
	if (!tmp)
	{
		printf("ERROR HANDLE_RIGHT, NO TOKEN\n");
		return (token);
	}
	if (tmp->type == RIGHT)
	{
		append = 1;
		if (tmp->next)
			tmp = tmp->next;
	}
	while (tmp->type == VOID && tmp->next)
		tmp = tmp->next;
	if (tmp->type != COMMAND)
	{
		printf ("REDIRECT FAILURE, NO FILENAME GIVEN\n");
		return (token);
	}
	else
		printf ("opening file [%s]\n", tmp->data);
	if (append == 1)
	{
		printf ("OPENING IN APPEND MODE\n");
		cmd->outfile = open(tmp->data, O_RDWR | O_APPEND | O_CREAT, 0644);
	}
	else
		cmd->outfile = open(tmp->data, O_RDWR | O_TRUNC | O_CREAT, 0644);
	printf("cmd outfile fd = [%i]\n", cmd->outfile);
	// if (token->prev)
		// printf("token prev = [%s]", token->prev->data);
		// tmp->prev = token->prev;
	// printf ("TEST: [%s]\n", token->prev->data);
	tmp->type = OUTFILE;
	if (!token->prev) //ugly fix, but the problem is if token happens to be the HEAD (shell->tokens), we will segf.
	{
		printf ("NEW HEAD INSTALLED\n");
		shell->tokens = tmp;
	}
	free_tokens_til(token, tmp);
	return (tmp);
}

/*
	this function will have to be split into an expansion and a real parsing function
*/
void parse_token(t_minishell *shell)
{
	t_token		*token;
	t_command	*cmd;
	int			cmd_finished = 0;
	int			i;

	i = 0;
	token = shell->tokens;
	cmd = ft_calloc(1, sizeof(t_command));
	cmd->outfile = STDOUT_FILENO;
	while (token)
	{
		// print_tokens(shell); //for testing purposes.
		if (token->type == LEFT)
			token = handle_left(token, shell);
		if (token->type == RIGHT)
			token = handle_right(token, shell, cmd);
		if (token->type == QUOTE)
			token = handle_quote(token);
		if (token->type == DOLLAR)
			expand_dong(token);
		if (token->type == COMMAND && cmd->command == NULL)
			cmd->command = ft_strdup(token->data);
		if (cmd->command && cmd->options == NULL && token->type == COMMAND)
			cmd->options = get_command_options(token); //todo: free this later.
		token = token->next;
	}
	if (cmd->command)
		execute(cmd, shell);
	// if (cmd->command)
		// free (cmd->command); // not sure when to free.
	// free (cmd); // not sure when to free.
}

void	insert_prompt(t_minishell	*shell)
{
	shell->command = readline("> ");
}

void	 sighandler(int signum)
{
	if(signum == SIGINT) // ctrl + C
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}

	//WHEN ALL DONE CAN REMOVING MEN
	exit(1);
}

int	init_minishell(t_minishell *shell, char **envp)
{
	//todo: add things to initialize? kek

	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
	shell->envp = envp;
	init_env(shell, envp);
	return (0);
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
		if (shell->command[0] == 'q' && ft_strlen(shell->command) == 1)
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
