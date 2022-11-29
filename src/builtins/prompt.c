/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/24 21:22:53 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/29 21:04:13 by wkonings      ########   odam.nl         */
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
	else if (shell->prompt_type == RACOON)
		shell->prompt = RACOON1 RACOON2;
	else if (shell->prompt_type == DOOKIE)
		shell->prompt = DOOKIE1 DOOKIE2;
	else if (shell->prompt_type == SHARK)
		shell->prompt = SHARK1 SHARK2;
	else if (shell->prompt_type == OKAY)
		shell->prompt = OKAY1 OKAY2;
	else if (shell->prompt_type == GORILA)
		shell->prompt = GORILA1 GORILA2;
	else if (shell->prompt_type == CHERRY)
		shell->prompt = CHERRY1 CHERRY2;
	else if (shell->prompt_type == DOOKIE)
		shell->prompt = DOOKIE1 DOOKIE2;
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
	else if (ft_strcmp(prompt[1], "dragon") == 0)
		shell->prompt_type = DOOKIE;
	else if (ft_strcmp(prompt[1], "okay") == 0)
		shell->prompt_type = OKAY;
	else if (ft_strcmp(prompt[1], "racoon") == 0)
		shell->prompt_type = RACOON;
	else if (ft_strcmp(prompt[1], "cherry") == 0)
		shell->prompt_type = SPOOKY;
	else if (ft_strcmp(prompt[1], "shark") == 0)
		shell->prompt_type = SHARK;
	else if (ft_strcmp(prompt[1], "care") == 0)
		shell->prompt_type = OKAY;
	else if (ft_strcmp(prompt[1], "monke") == 0)
		shell->prompt_type = GORILA;
	else
		return (false);
	return (true);
}

bool	change_prompt(t_command *cmd, t_minishell *shell)
{
	if (cmd && cmd->command[1] && prompt_choose(cmd->command, shell))
		return (set_prompt(shell));
	else if (cmd && cmd->command[1])
		printf (PROMPT_ALL);
	else
		shell->prompt_type++;
	if (shell->prompt_type > PROMPTS)
		shell->prompt_type = PROMPT_DEFAULT;
	return (set_prompt(shell));
}
