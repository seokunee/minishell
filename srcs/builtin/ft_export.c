/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seokchoi <seokchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 23:33:15 by seokchoi          #+#    #+#             */
/*   Updated: 2022/12/08 14:55:42 by seokchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	compare_str(char *str1, char *str2)
{
	size_t	i;

	i = 0;
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] - str2[i] < 0)
			return (1);
		else if (str1[i] - str2[i] > 0)
			return (0);
		i++;
	}
	if (str1[i] == '\0')
		return (1);
	else
		return (0);
}

void	sort_env(char **env, int env_len)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (env[i] && i < env_len - 1)
	{
		j = i + 1;
		while (env[j] && j < env_len)
		{
			if (compare_str(env[i], env[j]) == 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_export(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		printf("declare -x ");
		printf("%s\n", cmd[i]);
		i++;
	}
}

void	check_export_arg_right(t_token *token)
{
	char	*key;
	char	*value;

	if (ft_split_for_env(token->cmd[1], &key, &value) == FAIL)
	{
		errno = 1;
		return ;
	}
	ft_putenv(token->info->env_list, key, value);
	errno = 0;
}

void	ft_export(t_token *token)
{
	t_env_list	*env_list;
	int			cmd_len;
	char		**env;

	cmd_len = get_sec_arr_len(token->cmd);
	env_list = token->info->env_list;
	if (cmd_len == 1)
	{
		env = get_env_arr(env_list);
		sort_env(env, get_env_num(env_list));
		print_export(env);
		free_sec_arr(env);
		errno = 0;
		return ;
	}
	if (token->cmd[1])
		check_export_arg_right(token);
}
