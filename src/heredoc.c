/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:19:23 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/01 16:05:04 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


//todo: rewrite heredoc so it can handle creating multiple files with the same name.
//todo: create a delete_heredocfiles function or smth like that to cleanup after the heredoc.


char	*hd_count(t_minishell	*shell)
{
	int		i;
	char	*cwd;
	char	*ret_num;

	cwd = getcwd(NULL, 0);

	printf("CWD : %s", cwd);
	// if(ft_strcmp(cwd, ))
	ret_num = ft_itoa(shell->hd_count);
	ret_num = ft_strjoin("hd", ret_num);
	ret_num = ft_strjoin("bin/", ret_num);
	printf("Ret_str = %s\n", ret_num);
	return (ret_num);
}

t_token	*heredoc(t_token *token, t_minishell *shell)
{
	char	*delim;
	char	*line;
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

	fd = open(hd_count(shell), O_RDWR | O_CREAT | O_TRUNC, 0644);
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
	shell->hd_count++;
	return (tmp);
}
