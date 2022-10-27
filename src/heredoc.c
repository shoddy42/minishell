/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:19:23 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/27 05:05:05 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*heredoc(t_token    *token, t_minishell *shell)
{
	char	*delim;
	char	*line;
	t_token	*tmp;
	int		fd;

	tmp = token->next;
	while (tmp && tmp->type == VOID)
		tmp = tmp->next;
	fd = open(token->data, O_CREAT);
	printf("heredoc token: [%s]\n", tmp->data);
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
	printf("heredoc token: [%s]\n", tmp->data);
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