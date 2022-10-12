/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 20:13:59 by wkonings      #+#    #+#                 */
/*   Updated: 2022/02/24 16:23:14 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>

typedef struct s_ppx
{
	char	**path;

	int		tunnel[2];
	int		pid;
	int		here_doc;
	int		infile_fd;
	int		outfile_fd;
}				t_ppx;

# define FIRST 0
# define REGULAR 1
# define HEREDOC 2

# define WRITE 0
# define PERROR 1

		/* pipex specific functions */
char	*pipex_pathjoin(char const *path, char const *cmd);
void	pipex_error(char *error_message, int mode);
int		pipex_heredoc(char *av, t_ppx *pipex);
void	pipex_open(int ac, char **av, t_ppx *pipex);
int		split_path(char *env[], t_ppx *pipex);
int		get_fd_in(int current_fd, t_ppx *pipex, int mode);

		/* libft funtions */
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
int		get_next_line(int fd, char **line);

#endif
