/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/14 04:56:24 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// this function is not properly protected, might need to exit instead of return?
char	*pipex_pathjoin(char const *path, char const *cmd)
{
	char	*ret;
	size_t	i;
	size_t	j;

	if (!path || !cmd)
		return (NULL);
	ret = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(cmd) + 2));
	if (!ret)
		return (NULL);
	i = -1;
	j = -1;
	while (path[++i])
		ret[i] = path[i];
	while (cmd[++j] && cmd[j] != ' ')
		ret[i + j + 1] = cmd[j];
	ret[i] = '/';
	ret[i + j + 1] = '\0';
	return (ret);
}

/*
this function is way too basic. we'll need split
the forking to a different process most likely.
we also cant call this directly from the command generator,
since we need to check for pipe BEFORE.
*/
void    execute(t_command *cmd, t_minishell *shell)
{
	int		i;
	char	*path;
	pid_t	child;


	child = fork();
	i = 0;
	if (access(cmd->options[0], X_OK) == 0 && child == 0)
	{
		execve(cmd->options[0], cmd->options, shell->envp);
		printf("we in!\n");
	}
	while (shell->path[i] && child == 0)
	{
		path = pipex_pathjoin(shell->path[i], cmd->command);
		if (access(path, X_OK) == 0)
			execve(path, cmd->options, shell->envp);
		free(path);
		i++;
	}
	if (child == 0)
		exit(1);
	waitpid(child, NULL, 0);
	// rl_redisplay();
	// printf("\n");
}

char	**get_command_options(t_token	*token)
{
	char	**options;
	t_token	*tmp;
	int		i;

	tmp = token;
	i = 0;
	while (tmp && tmp->type == COMMAND)
	{
		i++;
		tmp = tmp->next;
	}
	options = ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while (token && token->type == COMMAND)
	{
		options[i] = ft_strdup(token->data);
		// printf("(%i) opt: [%s]\n", i, options[i]);
		token = token->next;
		i++;
	}
	return (options);
}

void parse_token(t_minishell *shell)
{
	t_token		*token;
	t_token		tmp;
	t_command	*cmd;
	int			cmd_finished = 0;
	int			i;

	i = 0;
	token = shell->tokens;
	cmd = ft_calloc(1, sizeof(t_command));
	while (token)
	{
		if (token->type == COMMAND && cmd->command == NULL)
		{
			cmd->command = ft_strdup(token->data);
			printf("cmd: %s\n", cmd->command);
		}
		if (cmd->command && cmd->options == NULL)
			cmd->options = get_command_options(token);
		token = token->next;
	}
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
	// print_env(shell);
	while (tmp_exit == 0)
	{
		command = readline("> ");
		if (command[0] == 'q' && ft_strlen(command) == 1)
			tmp_exit = 1;
		ft_tokenize(shell, command);
		parse_token(shell);
		if (ft_strlen(command) > 0)
			add_history(command);
		free(command);
		print_tokens(shell);
		free_tokens(shell);
	}
	return (0);
}
