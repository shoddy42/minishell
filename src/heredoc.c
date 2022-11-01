/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:19:23 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/01 18:23:33 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


//todo: rewrite heredoc so it can handle creating multiple files with the same name.
//todo: create a delete_heredocfiles function or smth like that to cleanup after the heredoc.


char	*hd_count(t_minishell	*shell)
{
	char	*num;
	char	*str;
	char	*ret;

	num = ft_itoa(shell->hd_count);
	str = ft_strjoin("heredoc_file_", num);
	ret = ft_strjoin(shell->bin_dir, str);
	printf("hd_count ret = [%s]\n", ret);
	return (ret);
}

void	delete_heredocs(t_minishell *shell)
{
	char	*heredoc;

	printf ("heredoc count?: [%i]\n", shell->hd_count);
	while (shell->hd_count > 0)
	{
		shell->hd_count--;
		heredoc = hd_count(shell);
		unlink(heredoc);
		free(heredoc);
	}
}

t_token	*heredoc(t_token *token, t_minishell *shell)
{
	char	*delim;
	char	*line;
	char	*heredoc;
	t_token	*tmp;
	int		fd;

	tmp = token->next;
	printf ("hd = %s\n", tmp->data);
	if (tmp && tmp->type != COMMAND && tmp->type != VOID && tmp->type != VARIABLE)
	{
		ms_error("Syntax Error near heredoc.", -7, FALSE, shell);
		return (tmp);
	}
	while (tmp && tmp->type == VOID)
		tmp = tmp->next;
	printf ("opening [%s]\n", tmp->data);

	heredoc = hd_count(shell);
	fd = open(heredoc, O_RDWR | O_CREAT | O_TRUNC, 0644);
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
	fd = open(heredoc, O_RDONLY);
	if (fd < 0)
		ms_error("SOMEHOW LOST THE HEREDOC KEK", -2, FALSE, shell);
	tmp->fd = fd;
	tmp->type = HEREDOC_FILE;
	free(heredoc);
	shell->hd_count++;
	return (tmp);
}
