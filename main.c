/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/12 22:58:03 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_minishell *shell)
{
	t_token *list;

	list = shell->tokens;
	while (list && list->next)
	{
		if (list->data)
			free(list->data);
		list = list->next;
	}
	if (list)
		free(list);
	shell->tokens = NULL;
}

t_token	*get_last_token(t_token *list)
{
	t_token *tmp;

	tmp = list;
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	get_token_type(t_minishell *shell, t_token *token)
{
	if (token->data[0] ==  ' ')
		token->type = VOID;
	else if (token->data[0] ==  '|')
		token->type = PIPE;
	else if (token->data[0] ==  '$')
		token->type = DOLLAR;
	else if (token->data[0] ==  '&')
		token->type = AND;
	else if (token->data[0] ==  '<')
		token->type = LEFT;
	else if (token->data[0] ==  '>')
		token->type = RIGHT;
	else
		token->type = COMMAND;
}

void	new_token(t_minishell *shell, char *data, int len)
{
	int		i;
	t_token	*new;
	t_token *last;

	i = -1;
	new = ft_calloc(1, sizeof(t_token));
	new->data = ft_calloc(len + 1, sizeof(char));
	while (++i < len)
		new->data[i] = data[i];
	last = get_last_token(shell->tokens);
	if (last != NULL)
	{
		last->next = new;
		new->prev = last;
	}
	else
		shell->tokens = new;
	get_token_type(shell, new);
}


//only issue with this so far is that for the final token, it'll allocate 1 bit too many.
void	ft_tokenize(t_minishell *shell, char *command)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (command[i])
	{
		while (ft_charinstr(DELIMITER, command[i]) == 0 && command[i])
			i++;
		if (ft_charinstr(DELIMITER, command[i]) == 1 && i > 0)
			i--;
		if (!(command[0] == ' ' && i == 0))
			new_token(shell, command, i + 1);
		command += i + 1;
		i = 0;
	}
}

void sighandler(int signum)
{
	printf("caught signal: %i\n", signum);
	//TODO: actual handling of specific signals.

	// remove the exit later
	exit(69);
}

void print_tokens(t_minishell *shell)
{
	int i = 0;
	t_token *test;

	test = shell->tokens;
	printf("all tokens: ");
	while (test && test->next)
	{
		printf("(%i)", test->type);
		printf("[%s]-", test->data);
		test = test->next;
	}
	if (test)
	{
		printf("(%i)", test->type);
		printf("[%s]\n", test->data);
	}
}

int	init_minishell(t_minishell *shell)
{
	//todo: add things to initialize? kek

	signal(SIGINT, sighandler);
	
	//todo: make function that gets env probably with getenv command instead of thru main.
	
	// printf("first?: [%s]\n", shell->tokens->data);
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
		// printf("LOOPING\n\n");
		command = readline("> ");
		if (command[0] == 'q' && ft_strlen(command) == 1)
			tmp_exit = 1;
		ft_tokenize(&shell, command);
		if (ft_strlen(command) > 0)
			add_history(command);
		free(command);
		print_tokens(&shell);
		free_tokens(&shell);
	}
	return (0);
}
