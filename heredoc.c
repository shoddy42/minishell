#include "minishell.h"


void    heredoc_loop(t_token    *token, t_minishell *shell)
{
    char    *delim;
    char    *line;
    t_token *tmp;
    int fds[2];

    tmp = token->next;
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
        write(fds[1], '\n', 1);
        free(line);
    }

}

void    ms_heredoc(t_token  *token, t_minishell *shell)
{
    int pid;




    pid = fork();
    if (pid == 0)
        heredoc_loop(token, shell);
}