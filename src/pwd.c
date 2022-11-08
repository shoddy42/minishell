#include  "../include/minishell.h"

int	ms_pwd(t_command	*cmd)
{
	int		i;
	char	*path;

	path = getcwd(NULL, 0);
	ft_putstr_fd(path, 1);
	write(1, "\n", 1);
	return (0);
}