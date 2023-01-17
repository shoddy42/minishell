/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:19:23 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/17 16:54:31 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*find_delim(t_token *token, t_minishell *shell)
{
	t_token	*hd;

	hd = token;
	while (hd && hd->next)
	{
		if (hd->next->type != VARIABLE && hd->next->type != COMMAND
			&& hd->next->type != DQUOTE && hd->next->type != QUOTE)
			break ;
		hd = hd->next;
	}
	if (hd && hd != token)
		hd = merge_tokens(token, hd, shell);
	return (hd);
}

t_token	*hd_delim(t_token *token, t_minishell *shell)
{
	t_token	*hd;
	char	*delim;
	int		skip;
	int		i;

	hd = find_delim(token, shell);
	if (!hd)
		return (token);
	delim = ft_calloc(ft_strlen(hd->data) + 1, sizeof(char));
	if (!delim)
		return (token_error(hd, "Allocing DELIM for heredoc failed. ["));
	i = -1;
	skip = 0;
	while (hd->data[++i + skip])
	{
		while (hd->data[i + skip] == '\'' || hd->data [i + skip] == '\"')
			skip++;
		if (hd->data[i + skip])
			delim[i] = hd->data[i + skip];
	}
	free (hd->data);
	hd->data = delim;
	return (hd);
}

void	hd_child(int fd, char *delim, t_minishell *shell)
{
	char	*line;
	int		i;

	while (1)
	{
		i = -1;
		line = readline(HD1 HD2);
		if (!line || ft_strcmp(line, delim) == 0)
		{
			if (!line)
				write (fd, "\n", 1);
			exit (0);
		}
		while (line[++i])
		{
			if (line[i] == '$' && line[i + 1])
				i += hd_var(fd, line + i + 1, shell);
			else
				write (fd, &line[i], 1);
		}
		write(fd, "\n", 1);
		if (shell->hd_history == true)
			add_history(line);
		free(line);
	}
}

t_token	*hd_open(char *heredoc, t_token *token, t_minishell *shell)
{
	pid_t	pid;
	int		fd;

	fd = open(heredoc, O_RDWR | O_CREAT | O_TRUNC, 0644);
	pid = fork();
	if (pid < 0 || fd < 0)
		return (token_error(token, "Creation of heredoc failed. ["));
	if (pid == 0)
	{
		if (shell->hd_history == true)
			rl_clear_history();
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, heredoc_sig);
	}
	else
		signal(SIGINT, SIG_IGN);
	if (pid == 0)
		hd_child(fd, token->data, shell);
	waitpid(pid, NULL, 0);
	close(fd);
	signal(SIGINT, sighandler);
	free(token->data);
	return (token);
}

t_token	*heredoc(t_token *start, t_minishell *shell)
{
	t_token	*token;
	char	*heredoc;

	if (start->next)
		token = start->next;
	else
		token = start;
	while (token && token->next && token->type == VOID)
		token = token->next;
	if (token->type != COMMAND && token->type != QUOTE
		&& token->type != DQUOTE && token->type != VARIABLE)
		return (token_error(token, SYN_NODELIM));
	token = hd_delim(token, shell);
	if (!token)
		return (token_error(start, "Getting delimiter failed ["));
	heredoc = get_hd_name(shell);
	hd_open(heredoc, token, shell);
	token->data = heredoc;
	token->type = HEREDOC;
	token->fd = shell->hd_count;
	shell->hd_count++;
	return (token);
}
