/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/09 20:44:04 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	get_token_type()

t_token	*ft_token_last(t_token *list)
{
	t_token *tmp;

	tmp = list;
	while(tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	get_token_type(t_minishell *shell, t_token *new)
{
	if (new->data[0] ==  ' ')
		new->type == VOID;
	if (new->data[0] ==  '|')
		new->type == PIPE;
	if (new->data[0] ==  '$')
		new->type == DOLLAR;
	if (new->data[0] ==  '&')
		new->type == AND;
	if (new->data[0] ==  '<')
		new->type == LEFT;
	if (new->data[0] ==  '>')
		new->type == RIGHT;
	if (new->data[0] ==  ' ')
		new->type == VOID;


}

void	new_token(t_minishell *shell, char *data, int len)
{
	int		i;
	t_token	*new;
	t_token *last;

	i = 0;
	printf("%i ", len);
	new = ft_calloc(sizeof(t_token), 1);
	new->data = ft_calloc(sizeof(char), len + 1);
	// new->data[0] = 'e';
	while (len-- > 0)
	{
		new->data[i] = data[i];
		i++;
	}
	new->data[i] = '\0';
	printf("test: [%s]\n", new->data);
	last = ft_token_last(shell->tokens);
	if (last)
	{
		last->next = new;
		new->prev = last;
	}
	// get_token_type(shell, new);
}

void	ft_tokenize_pointer(t_minishell *shell, char *command)
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
		if (command[i])
			new_token(shell, command, i + 1);
		command += i + 1;
		i = 0;
	}
}

void	ft_tokenize(t_minishell *shell, char *command)
{
	int	i;
	int	pointer;

	i = 0;
	pointer = 0;
	while (command[i + pointer])
	{
		// while (ft_charinstr(DELIMITER, command[i + pointer]) == 0 && command[i])
			// i++;
		// printf("%c ", command[pointer + i]);
		if (ft_charinstr(DELIMITER, command[i + pointer + 1]) == 1 || command[i + pointer + 1] == '\0')
		{
			printf("del? [%c] \n", command[pointer + i + 1]);
			new_token(shell, command, i + 1);
			if (i == 0)
				pointer++;
			pointer += i;
			i = 0;
		}
		i++;
	}
}


int	init_minishell(t_minishell *shell)
{
	//todo: add shit to initialize?
	return (0);
}

void print_tokens(t_minishell *shell)
{
	int i = 0;
	t_token *test;

	test = shell->tokens;
	printf("all tokens: ");
	while (test && test->next)
	{
		printf("[%s]\n", test->data);
		test = test->next;
	}
	printf("[%s]\n", test->data);
}

int	main(void)
{
	char		*command;
	int			tmp_exit = 0;
	t_minishell	shell;

	init_minishell(&shell);
	// while (ft_strcmp(command, "exit") && ft_strcmp(command, "q"))
	while (tmp_exit == 0)
	{
		command = readline("> ");
		if (command[0] == 'q' && ft_strlen(command) == 1)
			tmp_exit = 1;
		ft_tokenize_pointer(&shell, command);
		if (ft_strlen(command) > 0)
			add_history(command);
		free(command);
		// print_tokens(&shell);
	}
	return (0);
}
