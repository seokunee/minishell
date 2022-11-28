/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 08:19:19 by kko               #+#    #+#             */
/*   Updated: 2022/11/26 20:12:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*cmd_tree(t_token *tok)
{
	t_token	*cmd;
	t_token	*tmp;
	int		flag;

	cmd = new_token();
	cmd->type = TRDYCMD;
	cmd->line = "cmd용 빈파일"; //줄용량때문에 삭제예정
	tmp = tok;
	flag = 0;
	while (tmp)
	{
		if (tmp->type != TCMD)
			flag = 1; //flag 1이면 리다이렉션이존재
		if (tmp->type == TCMD)
			break ;
		tmp = tmp->next;
	}
	cmd->left = 0;
	if (flag == 1)
	{
		tmp->prev->next = 0;
		tmp->prev = 0;
		cmd->left = tok;
	}
	cmd->right = tmp;
	return (cmd);
}

t_token	*get_tree(t_token *token)
{
	t_token		*tmp;
	t_oper_type	oper1;
	t_oper_type	oper2;
	t_oper_type	oper3;

	oper1 = 0;
	oper2 = 0;
	oper3 = 0;
	tmp = token;
	select_oper(token, &oper1, &oper2, &oper3);
	if (oper1 == 0 && (token->type == TIN || token->type == TOUT || \
	token->type == TDOC || token->type == TADDOUT || token->type == TCMD))
		return (cmd_tree(ft_tokenstart(token)));
	while (tmp)
	{
		if (tmp->type == oper1 || tmp->type == oper2 || tmp->type == oper3)
		{
			tmp->left = get_tree(prev_token(tmp));
			tmp->right = get_tree(next_token(tmp));
			return (tmp);
		}
		tmp = tmp->prev;
	}
	return (token);
}

void	extra_work_tree(t_token *tok) //괄호처리 작업예정.
{
	t_token	*tmp;

		return ;
	if (tok == 0)
	tmp = tok;
	while (tmp)
	{
		if (tmp->type == TCMD)
			cmd_tree(tok);
		tmp = tmp->next;
	}
	extra_work_tree(tok->left);
	extra_work_tree(tok->right);
}

t_token	*next_token(t_token *token)
{
	token = token->next;
	token->prev->next = 0;
	token->prev = 0;
	return (token);
}

t_token	*prev_token(t_token *token)
{
	token = token->prev;
	token->next->prev = 0;
	token->next = 0;
	return (token);
}

void	select_oper(t_token *tok, t_oper_type *oper1, \
	t_oper_type *oper2, t_oper_type *oper3)
{
	t_token	*tmp;

	tmp = tok;
	while (tmp)
	{
		if (tmp->type == TDAND || tmp->type == TOR || tmp->type == TSEMI)
		{
			*oper1 = TDAND;
			*oper2 = TOR;
			*oper3 = TSEMI;
			return ;
		}
		tmp = tmp->prev;
	}
	tmp = tok;
	while (tmp)
	{
		if (tmp->type == TPIPE)
		{
			*oper1 = TPIPE;
			return ;
		}
		tmp = tmp->prev;
	}
}

