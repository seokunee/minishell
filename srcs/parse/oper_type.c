/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper_type.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seokchoi <seokchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:43:50 by seokchoi          #+#    #+#             */
/*   Updated: 2022/11/25 12:34:39 by seokchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_subshells(t_token **token)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while ((*token)->line[i])
	{
		if ((*token)->line[i] == '(')
		{
			(*token)->type = TBRACH;
			flag = 1;
			break ;
		}
		i++;
	}
	i = 0;
	while ((*token)->line[i] && flag == 1)
	{
		if ((*token)->line[i] != '(' && \
		((*token)->line[i] != ' ' && (*token)->line[i] != 0))
			throw_error(SYNTAX_ERR);
		else if ((*token)->line[i] == '(')
			new_push_index_until_space((*token)->line + i, &i, O_BRACHEK);
		i++;
	}
}

int	have_brachek(char *line)
{
	while (*line)
	{
		if (*line == '(')
			return (1);
		line++;
	}
	return (0);
}

void	check_type(t_token **token)
{
	set_type(token, '|', TPIPE, TOR);
	set_type(token, '&', NO_TYPE, TDAND);
	set_type(token, '<', TIN, TDOC);
	set_type(token, '>', TOUT, TADDOUT);
	if ((*token)->type == NO_TYPE)
		throw_error(SYNTAX_ERR);
}

void	set_type(t_token **token, char oper, t_oper_type one, t_oper_type two)
{
	char	*line;

	line = (*token)->line;
	if (line[0] == oper)
	{
		if (line[1] == 0 || check_operator(line[1]) == 0)
			(*token)->type = one;
		else if (line[1] == oper && check_operator(line[2]) == 0)
			(*token)->type = two;
	}
}

t_oper_type	check_operator(char c)
{
	if (c == '|')
		return(TPIPE);
	else if (c == '&')
		return (TAND);
	else if (c == '<')
		return (TIN);
	else if (c == '>')
		return (TOUT);
	else if (c == ';')
		return(TSEMI);
	return (NO_TYPE);
}

t_oper_type	first_check_operator(char c)
{
	if (c == '|')
		return (TPIPE);
	else if (c == '&')
		return (TAND);
	else if (c == ';')
		return (TSEMI);
	return (NO_TYPE);
}