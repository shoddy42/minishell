#include "minishell.h"
#include <stdio.h>

//should put the key of each line in ENV in tmp->key
//and also put each line of the key into tmp->beans

char    env_key(char   *envl)
{
    int     i;
    char    ret;

    i = 0;
    ret = malloc(ft_strlen(envl) * sizeof(char));
    while (i != '=')
    {
        ret[i] = envl[i];
        i++;
    }
    return (ret);
}

char    env_beans(char  *envl)
{
    int     i;
    int     j;
    char    ret;

    j = 0;
    i = 0;
    ret = malloc(ft_strlen(envl) * sizeof(char));
    while (envl[i]!= '=')
        i++;
    i++;
    while(envl[i])
    {
        ret[j] = envl[i];
        i++;
        j++;
    }
    return (ret);
}

void env_init(char  **envp)
{
    t_env   *tmp;
    int j;

    j = 0;

    while(j != 0)
    {
        tmp->key = env_key(envp[j]);
        tmp->beans = env_beans(envp[j]);
        tmp->next = tmp->next;
        j++;
    }
}