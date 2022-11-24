/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/24 21:22:53 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/24 23:23:38 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	set_prompt(t_minishell *shell)
{
	if (shell->prompt_type == TURTLE)
		shell->prompt = TURTLE1 TURTLE2;
	else if (shell->prompt_type == MOON)
		shell->prompt = MOON1 MOON2;
	else if (shell->prompt_type == HELL)
		shell->prompt = HELL1 HELL2;
	else if (shell->prompt_type == DRAGON)
		shell->prompt = DRAGON1 DRAGON2;
	else if (shell->prompt_type == SPOOKY)
		shell->prompt = SPOOKY1 SPOOKY2;
	return (0);
}

int	prompt_choose(char **prompt, t_minishell *shell)
{
	if (ft_strcmp(prompt[1], "turtle") == 0)
		shell->prompt_type = TURTLE;
	else if (ft_strcmp(prompt[1], "moon") == 0)
		shell->prompt_type = MOON;
	else if (ft_strcmp(prompt[1], "hell") == 0)
		shell->prompt_type = HELL;
	else if (ft_strcmp(prompt[1], "spooky") == 0)
		shell->prompt_type = SPOOKY;
	else if (ft_strcmp(prompt[1], "") == 0)
		shell->prompt_type = SPOOKY;
	else if (ft_strcmp(prompt[1], "") == 0)
		shell->prompt_type = MOON;
	else if (ft_strcmp(prompt[1], "") == 0)
		shell->prompt_type = HELL;
	else if (ft_strcmp(prompt[1], "") == 0)
		shell->prompt_type = SPOOKY;
	else if (ft_strcmp(prompt[1], "") == 0)
		shell->prompt_type = SPOOKY;
	else if (ft_strcmp(prompt[1], "") == 0)
		shell->prompt_type = MOON;
	else if (ft_strcmp(prompt[1], "") == 0)
		shell->prompt_type = HELL;
	else
		return (false);
	return (true);
}

bool	change_prompt(t_command *cmd, t_minishell *shell)
{
	if (cmd && cmd->command[1] && prompt_choose(cmd->command, shell))
		return (set_prompt(shell));
	else if (cmd && cmd->command[1])
		printf ("Available prompts: turtle, moon, hell, spooky\n");
	else
		shell->prompt_type++;
	if (shell->prompt_type > PROMPTS)
		shell->prompt_type = TURTLE;
	return (set_prompt(shell));
}