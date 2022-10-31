/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:19:23 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/31 12:09:18 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


//todo: rewrite heredoc so it can handle creating multiple files with the same name.
//todo: create a delete_heredocfiles function or smth like that to cleanup after the heredoc.
t_token	*heredoc(t_token *token, t_minishell *shell)
{
	char	*delim;
	char	*line;
	t_token	*tmp;
	int		fd;

	tmp = token->next;
	if (tmp && tmp->type != COMMAND && tmp->type != VOID)
	{
		ms_error("Syntax Error near heredoc.", -7, FALSE, shell);
		return (tmp);
	}
	while (tmp && tmp->type == VOID)
		tmp = tmp->next;
	printf ("opening [%s]\n", tmp->data);
	fd = open(tmp->data, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		ms_error("HEREDOC FAILED TO OPEN.", -1, FALSE, shell);
	printf("heredoc token: [%s] FD: [%i]\n", tmp->data, fd);
	delim = ft_strdup(tmp->data);
	while (1)
	{
		line = readline("heredoc> ");
		if(ft_strcmp(line, delim) == 0)
		{
			free(line);
			free(delim);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close (fd);
	fd = open(tmp->data, O_RDONLY);
	if (fd < 0)
		ms_error("SOMEHOW LOST THE HEREDOC KEK", -2, FALSE, shell);
	tmp->fd = fd;
	tmp->type = HEREDOC_FILE;
	return (tmp);
}

void    heredoc_loop(t_token *token, t_minishell *shell)
{
	char	*delim;
	char	*line;
	t_token	*tmp;
	int		file;
	int		fds[2];

	tmp = token->next;
	while (tmp && tmp->type == VOID)
		tmp = tmp->next;
	file = open(token->data, O_CREAT);
	printf("heredoc token: [%s] hd FD: [%i]\n", tmp->data, file);
	pipe(fds);
	delim = ft_strdup(tmp->data);
	while (1)
	{
		line = readline("heredoc> ");
		if(ft_strcmp(line, delim) == 0)
			{
				free(line);
				free(delim);
				break;
			}
		write(fds[1], line, ft_strlen(line));
		write(fds[1], "\n", 1);
		free(line);
	}
}

void    ms_heredoc(t_token  *token, t_minishell *shell)
{
	int	pid;

	pid = fork();
	if (pid == 0)
		heredoc_loop(token, shell);
}