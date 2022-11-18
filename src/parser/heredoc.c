/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 10:19:23 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/17 13:27:53 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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


//move to cleanup
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

void	heredoc_sig(int signum)
{
	if (signum == SIGINT)
	{
		printf ("sig int recieved\n");
		exit (1);
	}
}

//todo: MAYBE make it if there's quotes in the delimiter, dont expand variables.
//todo: check for potential segfaults if malloc fails.
//todo: if there is no (atm its obj, but it should be bin) folder, gotta make one.

t_token *hd_delim(t_token *token, t_minishell *shell)
{
	t_token	*hd;
	char	*delim;
	int		skip;
	int		i;

	hd = token;
	while (hd && hd->next && (hd->type == VARIABLE || hd->type == DQUOTE || hd->type == QUOTE || hd->type == COMMAND || hd->type == VOID))
		hd = hd->next;
	if (hd && hd != token)
		hd = merge_tokens(token, hd, shell);
	delim = ft_calloc(ft_strlen(hd->data), sizeof(char));
	if (!delim)
		return (token_error(hd, "Allocating delimiter for HD failed. [", true));
	i = 0;
	skip = 0;
	while (hd->data[i + skip])
	{
		if (hd->data[i + skip] == '\'' || hd->data [i + skip] == '\"')
			skip++;
		if (hd->data[i + skip])
			delim[i] = hd->data[i + skip];
		i++;
	}
	delim[i] = '\0';
	free (hd->data);
	hd->data = delim;
	printf ("delim = [%s]\n", delim);
	return (hd);
}

t_token	*heredoc(t_token *start, t_minishell *shell)
{
	t_token	*token;
	// char	*heredoc;
	char	*delim;
	char	*line;
	int		fd;
	pid_t	pid;

	//token select. hopes for smth past >
	if (start->next)
		token = start->next;
	else
		token = start;
	while (token && token->next && token->type == VOID)
		token = token->next;
	if (token->type != COMMAND && token->type != QUOTE && token->type != DQUOTE && token->type != VARIABLE)
		return (token_error(token, "Syntax error near '<<'; No valid delimiter for heredoc. [", true));
	free(token->data);
	heredoc = hd_count(shell);
	token = hd_delim(token, shell);
	delim = token->data;
	fd = open(heredoc, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (token_error(token, "Creating of heredoc failed. [", true));
	pid = fork();
	if (pid < 0)
		return (token_error(token, "Forking for heredoc failed. [", true));
	if (pid == 0)
		signal(SIGINT, heredoc_sig);
	else
		signal(SIGINT, SIG_IGN);
	while (pid == 0)
	{
		line = readline("heredoc> ");
		if (!line)
			exit (1);
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			free(delim);
			exit (0);
		}
		if (ft_charinstr('$', line))
			line = hd_var_exp(line, shell);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	waitpid(pid, NULL, 0);
	signal(SIGINT, sighandler);
	close(fd);
	// fd = open(heredoc, O_RDONLY);

	token->type = HEREDOC;
	token->fd = fd;
	return (token);
}

// t_token	*heredoc(t_token *token, t_minishell *shell)
// {
// 	char	*delim;
// 	char	*line;
// 	char	*heredoc;
// 	t_token	*tmp;
// 	int		fd;

// 	tmp = token->next;
// 	while (tmp && tmp->next && tmp->type == VOID)
// 		tmp = tmp->next;
// 	if (tmp->type != COMMAND && tmp->type != VARIABLE) //can it even be a variable here?
// 	{
// 		ms_error("Syntax Error near heredoc.", -7, false, shell);
// 		tmp->type = ERROR;
// 		return (tmp);
// 	}
// 	heredoc = hd_count(shell);
// 	fd = open(heredoc, O_RDWR | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 	{
// 		tmp->type = ERROR;
// 		ms_error("HEREDOC FAILED TO OPEN.", -1, false, shell);
// 	}
// 	delim = ft_strdup(tmp->data);
// 	pid_t doc;

// 	//todo: for the love of god refactor heredoc..
// 	doc = fork();
// 	if (doc == 0)
// 		signal(SIGINT, heredoc_sig);
// 	else
// 		signal(SIGINT, SIG_IGN);
// 	while (1)
// 	{
// 		if (doc == 0)
// 		{
// 			line = readline("heredoc> ");
// 			if (!line)
// 				break ;
// 			if (ft_strcmp(line, delim) == 0)
// 			{
// 				free(line);
// 				free(delim);
// 				break ;
// 			}
// 			if (ft_charinstr('$', line))
// 				line = hd_var_exp(line, shell);
// 			write(fd, line, ft_strlen(line));
// 			write(fd, "\n", 1);
// 			free(line);
// 		}
// 		else
// 			break ;
// 	}
// 	if (doc == 0)
// 		exit (0);
// 	else
// 	{
// 		waitpid(doc, NULL, 0);
// 		signal(SIGINT, sighandler);
// 	}
// 	close (fd);
// 	fd = open(heredoc, O_RDONLY);
// 	if (fd < 0)
// 		ms_error("SOMEHOW LOST THE HEREDOC KEK", -2, false, shell);
// 	tmp->fd = fd;
// 	tmp->type = HEREDOC;
// 	free(heredoc);
// 	shell->hd_count++;
// 	// printf ("hd ret = [%s]\n", tmp->data);
// 	return (tmp);
// }
