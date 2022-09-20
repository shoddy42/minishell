/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/20 11:33:12 by wkonings      ########   odam.nl         */
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

	int k = -1;
	while (cmd->options[++k] && child == 0)
		printf("opt: [%s]\n", cmd->options[k]);
	if (cmd->options && access(cmd->options[0], X_OK) == 0 && child == 0)
		execve(cmd->options[0], cmd->options, shell->envp);
	while (shell->path[i] && child == 0)
	{
		path = pipex_pathjoin(shell->path[i], cmd->command);
		if (access(path, X_OK) == 0)
		{
			printf("its go time\n");
			execve(path, cmd->options, shell->envp);
		}
		free(path);
		i++;
	}
	if (child == 0)
		exit(1);
	waitpid(child, NULL, 0);
}

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
	// ft_bzero(str, ft_strlen(str));
	printf("expanded: %s\n", token->data);
	token->next = tmp;
	if (token->next->type == QUOTE)
		token->next = NULL;
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
		// 	handle_quote(token);
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
