// /* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/19 17:04:56 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_status;



/**
 * @brief Checks infront of, and behind the pipe
 *  to check whether there is a syntax error.
 * 
 * @param token A token of which token->type == PIPE 
 * @param shell The shell.
 */
void	check_pipe(t_token *token, t_minishell *shell)
{
	t_token	*forward;
	t_token	*behind;

	forward = token->next;
	behind = token->prev;
	if (!forward || !behind)
	{
		shell->cancel_all_commands = true;
		return ;
	}
	while (forward && forward->next && forward->type != COMMAND
		&& forward->type != PIPE)
		forward = forward->next;
	while (behind && behind->next && behind->type != COMMAND
		&& forward->type != PIPE)
		behind = behind->next;
	if (forward->type != COMMAND || behind->type != COMMAND)
		shell->cancel_all_commands = true;
	shell->cancel_command = false;
}

// function should be finished. unless we want to add semicolon support.
// incorperate this into parse_token, and change the function to check for syntax errors near |

int	free_commands(t_minishell *shell)
{
	t_command	*tmp;
	int			i;

	tmp = shell->cmd_head;
	if (!tmp)
		return (-12); //wtf?
	while (tmp)
	{
		i = -1;
		if (tmp->command)
		{
			while (tmp->command[++i])
				free(tmp->command[i]);
			free (tmp->command);
		}
		if (!tmp->next)
			break ;
		tmp = tmp->next;
		if (tmp->prev)
			free(tmp->prev);
	}
	if (tmp)
		free(tmp);
	shell->cmd_head = NULL;
	return (0);
}

/**
 * @brief		Executes a single commandline in minishell, then exits minishell.
 * 
 * @param shell The shell.
 * @param av	The arg variables of the call to ./minishell.
 * @returns 0 on success. 1 on failure (currently doesnt happen.)
 */
int	dash_c(t_minishell *shell, char **av)
{
	if (av[1] && ft_strcmp(av[1], "-c") == 0)
	{
		if (av[2])
			shell->command = av[2];
		else
			ms_error("NO COMMAND STR.", -9, true, shell);
		ft_tokenize(shell, shell->command);
		parse_token(shell);
		count_pipes(shell);
		make_commands(shell);
		execute_two_electric_boogaloo(shell);
		free_commands(shell);
		free_tokens(shell);
		delete_heredocs(shell);
		exit (g_status);
	}
	return (0);
}

//todo: make comand and prompt work.
bool	change_prompt(t_minishell *shell)
{
	char	*old;

	if (!shell->command)
		return (false);
	old = shell->prompt;
	// printf ("type? [%i]\n", shell->prompt_type);
	if (ft_strcmp(shell->command, "prompt") == 0 || shell->prompt_type == 0)
		shell->prompt_type++;
	if (ft_strcmp(shell->command, "turtle") == 0 || shell->prompt_type == 1)
		shell->prompt = AT BT;
	if (ft_strcmp(shell->command, "hell") == 0 || shell->prompt_type == 2)
		shell->prompt = HELL1 HELL2;
	if (ft_strcmp(shell->command, "dragon") == 0 || shell->prompt_type == 3)
		shell->prompt = DRAGON1 DRAGON2;
	if (ft_strcmp(shell->command, "spooky") == 0 || shell->prompt_type == 4)
		shell->prompt = SPOOKY1 SPOOKY2;
	if (ft_strcmp(shell->command, "moon") == 0 || shell->prompt_type == 5)
		shell->prompt = MOON1 MOON2;


	if (shell->prompt_type > 5)
		shell->prompt_type = 0;
	if (ft_strcmp(old, shell->prompt) != 0 && ft_strcmp(shell->command, "prompt") != 0)
		shell->prompt_type = -1;
	// if (ft_strcmp(old, shell->prompt) != 0)
	// 	return (true);	
	return (false);
}

//later: make more test cases and more todos :)
//later: make sure EVERY alloc is protected properly.
//todo: norme.
int	main(int ac, char **av, char **envp)
{
	t_minishell	*shell;

	shell = ft_calloc(1, sizeof(t_minishell));
	if (!shell)
		ms_error("FAILED TO ALLOCATE SHELL STRUCT, YOU HAVE LITERALLY 0 MEMORY LMAO", -1, true, NULL);
	init_minishell(shell, envp);
	dash_c(shell, av);
	shell->prompt = "WELCOME TO MINIS HELL > ";
	while (shell->exit == 0)
	{
		create_bin(shell);
		shell->hd_count = 0;
		shell->command = readline(shell->prompt);
		change_prompt(shell);
		// if (change_prompt(shell))
			// shell->cancel_all_commands = true;
		if (shell->command == NULL) // todo: make it so we actually write exit with rl_replace_line somehow
		{
			// printf ("test\n");
			rl_replace_line("minishell> exit", 0);
			rl_on_new_line();
			rl_redisplay();
			exit (1);
		}
		// printf ("Wtf? [%s] len?: [%zu]\n", shell->command, ft_strlen(shell->command));
		shell->command_len = ft_strlen(shell->command);
		if (shell->command[0] == 'q' && ft_strlen(shell->command) == 1) //temporary exits
			exit (0);
		if (ft_strcmp(shell->command, "exit") == 0)
			shell->exit = 1;
		ft_tokenize(shell, shell->command); //immune to error
		// print_tokens(shell);
		parse_token(shell); //if error, cut out everything
		// print_tokens(shell);
		count_pipes(shell);
		// printf ("how many?: [%i]\n", shell->hd_count);
		if (shell->cancel_all_commands == false)
		{
			if (make_commands(shell) == 0)
			{
				// print_commands(shell);
				execute_two_electric_boogaloo(shell);
			}
			// printf("$? [%i]\n", g_status); //print last cmd return
			printf("g$? [%i]\n", g_status); //print last cmd return
			// print_tokens(shell);
			// print_tokens_backwards(shell); //for testing whether prev is linked properly.

			//america AKA freeing all that needs freeing, resetting all that needs resetting.
			free_commands(shell);

			// reset signals after changing them in exe
			if (signal(SIGINT, sighandler) == SIG_ERR)
				exit (55);
		}
		else
			printf ("SYNTAXICAL ERROR\n"); //todo: make actual syntax error reporter, which will also work with <<<<< and >>>>>
		delete_heredocs(shell);
		free_tokens(shell);
		if (ft_strlen(shell->command) > 0)
			add_history(shell->command);
		if (shell->command)
			free(shell->command);
		shell->cancel_all_commands = false;
		shell->cancel_command = false;
	}
	return (0);
}
