/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/03 21:08:54 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/13 23:22:59 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipexec(char *cmd, char *envp[], t_ppx *pipex)
{
	char	**args;
	char	*path;
	int		i;

	args = ft_split(cmd, ' ');
	i = 0;
	if (access(args[0], X_OK) == 0)
		execve(args[0], args, envp);
	while (pipex->path[i])
	{
		path = pipex_pathjoin(pipex->path[i], cmd);
		if (access(path, X_OK) == 0)
		{
			execve(path, args, envp);
			exit (1);
		}
		free(path);
		i++;
	}
	write(2, "pipex: ", 7);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
	exit(127);
}

void	forking(char *cmd, char *envp[], t_ppx *pipex, int mode)
{
	int		tunnel[2];
	int		fd_in;
	int		fd_out;

	pipe(tunnel);
	pipex->pid = fork();
	fd_out = tunnel[1];
	if (mode == 0)
		fd_out = pipex->outfile_fd;
	fd_in = tunnel[0];
	fd_in = get_fd_in(fd_in, pipex, mode);
	if (pipex->pid == 0)
	{
		close(tunnel[0]);
		dup2(tunnel[1], STDOUT_FILENO);
		dup2(fd_in, STDIN_FILENO);
		pipexec(cmd, envp, pipex);
	}
	else
	{
		close (tunnel[1]);
		dup2(fd_out, STDOUT_FILENO);
		dup2 (tunnel[0], STDIN_FILENO);
		waitpid (pipex->pid, NULL, 0);
	}
}

void	read_heredoc(char *av, t_ppx *pipex)
{
	char	*limiter;
	char	*line;
	int		gnl;

	limiter = ft_strdup(av);
	close(pipex->tunnel[0]);
	write(2, "> ", 2);
	gnl = get_next_line(STDIN_FILENO, &line);
	while (gnl)
	{
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			free(limiter);
			exit(0);
		}
		write(pipex->tunnel[1], line, ft_strlen(line));
		write(pipex->tunnel[1], "\n", 1);
		write(2, "> ", 2);
		free(line);
		gnl = get_next_line(STDIN_FILENO, &line);
	}
	free(line);
	free(limiter);
}

int	pipex_heredoc(char *av, t_ppx *pipex)
{
	pid_t	pid;

	pipex->here_doc = 1;
	pipe(pipex->tunnel);
	pid = fork();
	if (pid > 0)
	{
		close(pipex->tunnel[1]);
		dup2(pipex->tunnel[0], STDIN_FILENO);
		close(pipex->tunnel[0]);
		waitpid(pid, NULL, 0);
	}
	else
		read_heredoc(av, pipex);
	return (0);
}

int	main(int ac, char **av, char *envp[])
{
	int		i;
	t_ppx	pipex;

	i = 2;
	if (ac < 5 || (ac < 6 && ft_strncmp("here_doc", av[1], 8) == 0))
		pipex_error("Not enough arguments.\n", WRITE);
	split_path(envp, &pipex);
	pipex_open(ac, av, &pipex);
	if (pipex.here_doc == 0)
		forking(av[2], envp, &pipex, FIRST);
	if (pipex.here_doc == 1)
	{
		forking(av[3], envp, &pipex, HEREDOC);
		i++;
	}
	while (++i < ac - 2)
		forking(av[i], envp, &pipex, REGULAR);
	dup2(pipex.outfile_fd, STDOUT_FILENO);
	// system("leaks pipex");
	pipexec(av[i], envp, &pipex);
	return (-1);
}
