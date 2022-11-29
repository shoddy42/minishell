/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/24 00:01:04 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/29 19:29:17 by wkonings      ########   odam.nl         */
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
	ret = pipex_pathjoin(shell->bin_dir, str);
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

int	hd_var(int fd, char *line, t_minishell *shell)
{
	char	*key;
	char	*var;
	int		var_index;
	int		i;

	key = ft_calloc(ft_strlen(line), sizeof(char));
	if (!key)
		exit (1);
	i = -1;
	while (line[++i] && (line[i] != ' ' && line[i] != '\t' && \
			line[i] != '\"' && line[i] != '\'' && line[i] != '$'))
		key[i] = line[i];
	var = ms_getenv(key, shell);
	var_index = -1;
	while (var[++var_index])
		write (fd, &var[var_index], 1);
	free (key);
	return (i);
}
