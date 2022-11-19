/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:19:23 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/19 01:35:26 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief get the full path to the heredoc file that should be present.
 * 
 * @param shell The shell.
 * @returns		An allocated string containing the full path.
 */
char	*get_hd_name(t_minishell	*shell)
{
	char	*num;
	char	*str;
	char	*ret;

	num = ft_itoa(shell->hd_count);
	str = ft_strjoin("heredoc_file_", num);
	ret = ft_strjoin(shell->bin_dir, str);
	free (num);
	free (str);
	return (ret);
}

/**
 * @brief Deletes all temporary heredoc_files in /bin/
 * 
 * @param shell The shell.
 */
void	delete_heredocs(t_minishell *shell)
{
	char	*heredoc;

	while (shell->hd_count > 0)
	{
		shell->hd_count--;
		heredoc = get_hd_name(shell);
		unlink(heredoc);
		free(heredoc);
	}
}

void	heredoc_sig(int signum)
{
	if (signum == SIGINT)
	{
		printf ("sig int recieved\n");
		exit (1);
	}
}

//todo: check for potential segfaults if malloc fails.
t_token *hd_delim(t_token *token, t_minishell *shell)
{
	t_token	*hd;
	char	*delim;
	bool	in_quotes;
	int		skip;
	int		i;

	hd = token;
	while (hd && hd->next)
	{
		if (hd->next->type != VARIABLE && hd->next->type != COMMAND && hd->next->type != DQUOTE && hd->next->type == QUOTE)
			break ;
		hd = hd->next;
	}
	if (hd && hd != token)
		hd = merge_tokens(token, hd, shell);
	delim = ft_calloc(ft_strlen(hd->data), sizeof(char));
	if (!delim)
		return (token_error(hd, "Allocating delimiter for heredoc failed. [", true));
	i = -1;
	skip = 0;
	while (hd->data[++i + skip])
	{
		while (hd->data[i + skip] == '\'' || hd->data [i + skip] == '\"')
			skip++;
		if (hd->data[i + skip])
			delim[i] = hd->data[i + skip];
	}
	delim[i] = '\0';
	printf ("delim = [%s]\n", delim);
	free (hd->data);
	hd->data = delim;
	return (hd);
}

int		hd_var(int fd, char *line, t_minishell *shell)
{
	char	*key;
	char	*var;
	int		var_index;
	int		i;

	printf ("expanding var [%s]\n", line);
	key = ft_calloc(ft_strlen(line), sizeof(char));
	if (!key)
		exit (1);
	i = -1;
	while (line[++i] && (line[i] != ' ' && line[i] != '\t' && line[i] != '\"' && line[i] != '\'' && line[i] != '$'))
		key[i] = line[i];
	var = ms_getenv(key, shell);
	var_index = -1;
	while (var[++var_index])
		write (fd, &var[var_index], 1);
	free (key);
	return (i);
}

void	hd_child(int fd, char *delim, t_minishell *shell)
{
	char	*line;
	int		i;

	printf ("DELIM = [%s]\n", delim);
	while (1)
	{
		i = -1;
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delim) == 0) //this was split at first
			exit (0);

		while (line[++i])
		{
			if (line[i] == '$' && line[i + 1])
				i += hd_var(fd, line + i + 1, shell);
			else //if (line[i])
				write (fd, &line[i], 1);
		}
		write(fd, "\n", 1);
		free(line);
	}
}

//todo: check leaks
//todo: fix sometingwong.
t_token	*heredoc(t_token *start, t_minishell *shell)
{
	t_token	*token;
	pid_t	pid;
	char	*heredoc;
	int		fd;

	//token select. hopes for smth past >
	if (start->next)
		token = start->next;
	else
		token = start;
	while (token && token->next && token->type == VOID)
		token = token->next;
	if (token->type != COMMAND && token->type != QUOTE && token->type != DQUOTE && token->type != VARIABLE)
		return (token_error(token, "Syntax error near '<<'; No valid delimiter for heredoc. [", true));
	token = hd_delim(token, shell); // what if token die?
	heredoc = get_hd_name(shell);
	fd = open(heredoc, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (token_error(token, "Creation of heredoc failed. If there is no [minishell]/bin/ dir, rerun make [", true));
	pid = fork();
	if (pid < 0)
		return (token_error(token, "Forking for heredoc failed. [", true));
	if (pid == 0)
		signal(SIGINT, heredoc_sig);
	else
		signal(SIGINT, SIG_IGN);
	//child writes the actual heredoc.
	if (pid == 0)
		hd_child(fd, token->data, shell);
	//parent waits and resets signals.
	waitpid(pid, NULL, 0);
	close(fd);
	signal(SIGINT, sighandler);

	//changes the data to be the heredoc_name instead.
	// printf ("hd data [%s]\n", token->data);
	free(token->data);
	token->data = heredoc;
	// printf ("hd data [%s]\n", token->data);
	token->type = HEREDOC;
	token->fd = shell->hd_count;
	shell->hd_count++;
	return (token);
}
