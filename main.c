/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/12 21:40:01 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tokens(t_minishell *shell)
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


t_token	*ft_token_last(t_token *list)
{
	t_token *tmp;

	tmp = list;
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	get_token_type(t_minishell *shell, t_token *new)
{
	if (new->data[0] ==  ' ')
		new->type = VOID;
	if (new->data[0] ==  '|')
		new->type = PIPE;
	if (new->data[0] ==  '$')
		new->type = DOLLAR;
	if (new->data[0] ==  '&')
		new->type = AND;
	if (new->data[0] ==  '<')
		new->type = LEFT;
	if (new->data[0] ==  '>')
		new->type = RIGHT;
	if (new->data[0] ==  ' ')
		new->type = VOID;
}

int	new_token(t_minishell *shell, char *data, int len)
{
	int		i;
	t_token	*new;
	t_token *last;

	i = -1;
	// printf("%i ", len);
	new = ft_calloc(sizeof(t_token), 1);
	new->data = ft_calloc(sizeof(char), len + 1);
	while (++i < len)
		new->data[i] = data[i];
	new->data[i] = '\0';
	// printf("data: [%s]\n", new->data);
	last = ft_token_last(shell->tokens);
	if (last != NULL)// && last->data != NULL)
	{
		last->next = new;
		new->prev = last;
	}
	else
		shell->tokens = new;
	get_token_type(shell, new);
	return (0);
}

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
	// remove the exit later
	// rl_replace_line("eee: ", 0);
	// rl_redisplay()
	// rl_on_new_line();
		// rl_on_new_line
	// rl_on_new_line
	// exit(1);
	// rl_redi
}

void print_tokens(t_minishell *shell)
{
	int i = 0;
	t_token *test;

	test = shell->tokens;
	// printf("shell: %s", shell->tokens->data);
	printf("all tokens: ");
	while (test && test->next)
	{
		printf("[%s]-", test->data);
		test = test->next;
	}
	if (test)
		printf("[%s]", test->data);
	// printf("[%s]\n", test->data);
}

int	init_minishell(t_minishell *shell)
{
	//first token for testing purposes
	// t_token *first;

	// first = ft_calloc(1, sizeof(t_token));
	// first->data = ft_strdup("test");
	// shell->tokens = first;
	//todo: add things to initialize? kek

	//todo: fix signals, to catch al control + X commands
	
	// signal(SIGINT, sighandler);
	
	//todo: make function that takes ENVP from main, and turns it into a linked list
	
	// printf("first?: [%s]\n", shell->tokens->data);
	return (0);
}

int	main(void)
{
	char		*command;
	int			tmp_exit = 0;
	t_minishell	shell;
	int		pass = 0; // remove when done testing


	init_minishell(&shell);
	while (tmp_exit == 0 && pass < 2)
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
		ft_free_tokens(&shell);
		// pass++;
	}
	return (0);
}
