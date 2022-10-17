/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/17 18:31:01 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_commands(t_minishell *shell)
{
	t_command	*tmp;
	int	cmd_num = 0;
	int i = 0;

	tmp = shell->commands;
	printf ("\nPrinting all commands:\n");
	while(tmp)
	{
		printf ("\nCOMMAND (%i)    infile [%i] outfile [%i]\n", cmd_num, tmp->infile, tmp->outfile);
		cmd_num++;
		i = 0;
		while (tmp && tmp->command && tmp->command[i])
		{
			printf("cmd(%i) = [%s]\n", i, tmp->command[i]);
			i++;
		}
		if (!tmp->next)
		{
			printf ("no command next\n");
			break;
		}
		tmp = tmp->next;
	}
}

t_token	*get_command_options(t_token *token, t_command *cmd)
{
	t_token		*tmp;
	int			i;

	tmp = token;
	i = 0;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == COMMAND)
			i++;
		tmp = tmp->next;
	}
	cmd->command = ft_calloc(i + 1, sizeof(char *));
	// printf ("allocated command with [%i]\n", i + 1);
	tmp = token;
	// printf ("token at get_command_options = [%s]\n", token->data);
	i = 0;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == COMMAND)
			cmd->command[i++] = ft_strdup(tmp->data);
		if (tmp->type == INFILE || tmp->type == HEREDOC_FILE)
			cmd->infile = tmp->fd;
		if (tmp->type == OUTFILE)
			cmd->outfile = tmp->fd;
		if (!tmp->next)
			break;
		tmp = tmp->next;
	}
	return (tmp);
}

t_command	*new_command(t_minishell *shell, t_command *cmd)
{
	t_command	*new;

	new = ft_calloc(1, sizeof(t_command));
	new->outfile = STDOUT_FILENO;
	new->infile = STDIN_FILENO;
	if (cmd != NULL)
	{
		new->prev = cmd;
		cmd->next = new;
	}
	return (new);
}

int	do_pipe_magic(t_minishell *shell, t_command *cmd)
{
	if (cmd->prev)
	{ 
		if (cmd->infile == STDIN_FILENO)
			cmd->infile = shell->tunnel[0];
		if (cmd->prev->outfile == STDOUT_FILENO)
			cmd->prev->outfile = shell->tunnel[1];
	}
	return (0);
}

//todo: FREE the commands, close the file descriptors.
int	make_commands(t_minishell *shell)
{
	t_command	*cmd;
	t_token		*token;
	int			i;
	int			pipe;

	cmd = new_command(shell, cmd);
	shell->commands = cmd;
	token = shell->tokens;
	while (token)
	{
		token = get_command_options(token, cmd);
		printf("returned token = [%s]\n", token->data);
		i = 0;
		if (token && token->type == PIPE)
		{
			printf ("PIPE FOUND \n");
			cmd = new_command(shell, cmd);
			if (do_pipe_magic(shell, cmd) == -1)
				exit(75);
		}
		token = token->next;
	}
	// if (cmd && cmd->command && cmd->command[0] != NULL)
	// 	execute(cmd, shell);
	return (0);
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
		if (token->type == LEFT)
			token = handle_left(token, shell);
		if (token->type == RIGHT)
			token = handle_right(token, shell);
		if (token->type == QUOTE || token->type == DQUOTE)
			token = handle_quote(token, token->type);
		if (token->type == VARIABLE)
			expand_dong(token);
		token = token->next;
	}
	make_commands(shell);
	print_commands(shell);
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
		execute_two_electric_boogaloo(shell);
		// print_tokens_backwards(shell);
		free_tokens(shell);
	}
	return (0);
}
