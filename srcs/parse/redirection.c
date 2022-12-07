/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kko <kko@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:59:30 by seokchoi          #+#    #+#             */
/*   Updated: 2022/12/07 13:08:57 by kko              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	attach_redir_token(t_token **token, \
t_token	**redir_token, char *file_name)
{
	t_token	*new;
	t_token	*tmp;

	tmp = *redir_token;
	new = new_token((*token)->info); // 새로운 토큰 생성
	new->line = ft_strdup(file_name); // <a
	check_type(&new);
	if (!new->line)
		throw_error(MALLOC_ERR);
	if (!(*redir_token))
	{
		(*redir_token) = new;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

static t_token	*pick_create_redir_tokens(t_token **token, char **cmd)
{
	int		i;
	t_token	*redir_token;

	i = 0;
	redir_token = NULL;
	while (cmd[i])
	{
		if (ft_is_redir(cmd[i][0]) != NO_DIREC)
			attach_redir_token(token, &redir_token, cmd[i]); // redir_token을 새로 만들어줘서 따로 정리 해준다.
		i++;
	}
	return (redir_token);
}

static char **pick_create_only_cmd_arr(char **arr, int only_cmd_len)
{
	char	**new_cmd_arr;
	int		i;
	int		k;

	if (only_cmd_len == 0)
		return (NULL);
	new_cmd_arr = malloc(sizeof(char *) * (only_cmd_len + 1));
	if (!new_cmd_arr)
		throw_error(MALLOC_ERR);
	new_cmd_arr[0] = NULL;
	k = 0;
	i = 0;
	while (arr[i])
	{
		if (ft_is_redir(arr[i][0]) == NO_DIREC)
		{
			new_cmd_arr[k] = ft_strdup(arr[i]);
			if (!new_cmd_arr[k])
				throw_error(MALLOC_ERR);
			k++;
		}
		i++;
	}
	new_cmd_arr[k] = NULL;
	return (new_cmd_arr);
}

static char	*update_token_line(t_token **token)
{
	int	i;
	char *new_line;
	char *tmp;

	i = 1;
	if (!(*token)->cmd)
		return (NULL);
	new_line = ft_strdup((*token)->cmd[0]);
	if (!new_line)
		return (NULL);
	while ((*token)->cmd[i])
	{
		tmp = new_line;
		new_line = ft_strjoin_space(new_line, (*token)->cmd[i]);
		free(tmp);
		i++;
	}
	return (new_line);
}

static void	sort_token_order(t_token **token, t_token **first, t_token *redir_token)
{
	t_token	*redir_last;
	t_token	*tmp_prev_token;

	tmp_prev_token = (*token)->prev;
	if (tmp_prev_token && redir_token)
	{
		tmp_prev_token->next = redir_token;
		redir_token->prev = tmp_prev_token;
		redir_last = ft_tokenlast(redir_token);
		(*token)->prev = redir_last;
		redir_last->next = (*token);
	}
	else if (redir_token)
	{
		redir_last = ft_tokenlast(redir_token);
		redir_last->next = (*token);
		(*token)->prev = redir_last;
		*first = redir_token;
	}
}

static void	devide_redir_cmd(t_token **token, t_token **first)
{
	t_token	*tmp_token;
	t_token	*redir_token;
	char	**new_cmd_arr;

	redir_token = pick_create_redir_tokens(token, (*token)->cmd);
	new_cmd_arr = pick_create_only_cmd_arr((*token)->cmd, get_sec_arr_len((*token)->cmd) - token_list_len(redir_token));
	free_sec_arr((*token)->cmd);
	(*token)->cmd = new_cmd_arr;
	// (*token)->type = TNOCMD;
	(*token)->line = update_token_line(token);
	sort_token_order(token, first, redir_token);
}

void	set_type_remove_operator(t_token **token, t_token **first)
{
	if (first_check_operator((*token)->line[0]) != NO_TYPE)
		check_type(token);
	else if (have_brachek((*token)->line, *token) != 0)
	{
		(*token)->type = TBRACH;
		if ((*token)->line[0] != '(')
			throw_error_syntax(SYNTAX_ERR, *token);
		check_subshells(token, 0);
	}
	else if (*token)
	{
		set_cmd(token);
		devide_redir_cmd(token, first);
	}
}
