/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 18:18:54 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/12 15:06:49 by auzochuk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	get_fd_in(int current_fd, t_ppx *pipex, int mode)
{
	int	fd_in;

	if (mode == 0)
		fd_in = pipex->infile_fd;
	if (mode == 1)
		fd_in = current_fd;
	if (mode == 2)
		fd_in = pipex->tunnel[0];
	return (fd_in);
}

void	pipex_open(int ac, char **av, t_ppx *pipex)
{
	if (ft_strncmp("here_doc", av[1], 8) == 0)
	{
		pipex->outfile_fd = open(av[ac - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
		pipex_heredoc(av[2], pipex);
	}
	else
	{
		pipex->outfile_fd = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		pipex->infile_fd = open(av[1], O_RDONLY, 0644);
	}
	if (pipex->infile_fd < 0)
		pipex_error("pipex: infile", PERROR);
	if (pipex->outfile_fd < 0)
		pipex_error("pipex: outfile", PERROR);
}

int	split_path(char *envp[], t_ppx *pipex)
{
	int	i;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0 && envp[i + 1])
		i++;
	if (ft_strncmp(envp[i], "PATH=", 5) != 0)
		exit (1);
	pipex->path = ft_split(envp[i] + 6, ':');
	return (0);
}

void	pipex_error(char *error_message, int mode)
{
	if (mode == WRITE)
		write (STDERR_FILENO, error_message, ft_strlen(error_message));
	if (mode == PERROR)
		perror (error_message);
	exit(1);
}
