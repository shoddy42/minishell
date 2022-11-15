/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:19:23 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/15 09:55:16 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// i think the entire heredoc is perfect now. just leave it until i get back
//later: make sure every instance where
//hd_count gets called, it's return value is freed before lost.
char	*hd_count(t_minishell	*shell)
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

void	delete_heredocs(t_minishell *shell)
{
	char	*heredoc;

	while (shell->hd_count > 0)
	{
		shell->hd_count--;
		heredoc = hd_count(shell);
		unlink(heredoc);
		free(heredoc);
	}
}

//todo: leaks and protection.
char	*hd_var_exp(char *line, t_minishell *shell)
{
	char	**split;
	char	*var;
	char	*ret;
	int		i;

	split = ft_split(line, ' ');
	if (!split)
		ms_error("Failed to heredoc noob", 1, true, shell);
	i = -1;
	ret = ft_calloc(1, 1);
	while (split[++i])
	{
		if (split[i][0] == '$')
		{
			var = ms_getenv(split[i] + 1, shell);
			free(split[i]);
			split[i] = ft_strdup(var);
		}
		ret = ft_strexpand(ret, split[i]);
		ret = ft_strexpand(ret, " ");
		free(split[i]);
	}
	free (split);
	return (ret);
}

//todo: MAYBE make it if there's quotes in the delimiter, dont expand variables.
//todo: check for potential segfaults if malloc fails.
t_token	*heredoc(t_token *token, t_minishell *shell)
{
	char	*delim;
	char	*line;
	char	*heredoc;
	t_token	*tmp;
	int		fd;

	tmp = token->next;
	while (tmp && tmp->next && tmp->type == VOID)
		tmp = tmp->next;
	if (tmp->type != COMMAND && tmp->type != VARIABLE) //can it even be a variable here?
	{
		ms_error("Syntax Error near heredoc.", -7, false, shell);
		tmp->type = ERROR;
		return (tmp);
	}
	heredoc = hd_count(shell);
	fd = open(heredoc, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		tmp->type = ERROR;
		ms_error("HEREDOC FAILED TO OPEN.", -1, false, shell);
	}
	delim = ft_strdup(tmp->data);
	while (1)
	{
		line = readline("heredoc> ");
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			free(delim);
			break ;
		}
		if (ft_charinstr('$', line))
			line = hd_var_exp(line, shell);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close (fd);
	fd = open(heredoc, O_RDONLY);
	if (fd < 0)
		ms_error("SOMEHOW LOST THE HEREDOC KEK", -2, false, shell);
	tmp->fd = fd;
	tmp->type = HEREDOC_FILE;
	free(heredoc);
	shell->hd_count++;
	return (tmp);
}
