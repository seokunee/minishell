/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_run.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kko <kko@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 20:22:40 by seokchoi          #+#    #+#             */
/*   Updated: 2022/12/12 20:02:38 by kko              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builitin(t_token *tok)
{
	if (tok->type != TCMD)
		return (-1);
	if (ft_strncmp(tok->cmd[0], "echo", 5) == 0)
		ft_echo(tok->cmd);
	else if (ft_strncmp(tok->cmd[0], "cd", 3) == 0)
		ft_cd(tok);
	else if (ft_strncmp(tok->cmd[0], "pwd", 4) == 0)
		ft_pwd(tok->cmd);
	else if (ft_strncmp(tok->cmd[0], "export", 7) == 0)
		ft_export(tok);
	else if (ft_strncmp(tok->cmd[0], "unset", 5) == 0)
		ft_unset(tok);
	else if (ft_strncmp(tok->cmd[0], "env", 4) == 0)
		ft_env(tok);
	else if (ft_strncmp(tok->cmd[0], "exit", 5) == 0)
		ft_exit(tok, tok->cmd);
	return (0);
}

void	builtin_alone_exec(t_token *tok)
{
	set_signal(DFL);
	if (tok->left->type != NO_REDIR)
		io_ctl_cmd(tok->left);
	if (tok->right->type == TNOCMD)
		return ;
	ft_builitin(tok->right);
	set_signal(BASH);
}
