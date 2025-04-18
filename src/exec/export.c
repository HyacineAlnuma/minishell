/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:32:35 by secros            #+#    #+#             */
/*   Updated: 2025/04/18 17:33:36 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_export_arg(char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
	{
		print_error("export", arg, ": not a valid identifier");
		return (0);
	}
	while (arg[++i])
	{
		if (arg[i] == '=')
			break ;
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			print_error("export", arg, ": not a valid identifier");
			return (0);
		}
	}
	return (1);
}

void	export_vars(t_exec *cmd, t_list **env, int i)
{
	t_list	*new_line;
	char	*env_line;

	if (!check_export_arg(cmd->opt[i]))
		return ;
	if (unset(cmd, env, i) == 1)
	{
		env_line = fill_dishwasher(ft_strdup(cmd->opt[i]), \
		free, get_sink(NULL));
		new_line = fill_dishwasher(ft_lstnew(env_line), \
		free, get_sink(NULL));
		if (!new_line || !new_line)
		{
			perror("malloc error");
			return ;
		}
		ft_lstadd_back(env, new_line);
	}
}

void	export(t_exec *cmd, t_list **env, int is_parent)
{
	int		i;

	i = 1;
	if (!cmd->opt[1] && !is_parent)
	{
		print_exp_env(env);
		clean_exit(cmd->bin, EXIT_SUCCESS);
	}
	while (cmd->opt[i])
	{
		if (cmd->opt[i] && is_parent)
			export_vars(cmd, env, i);
		i++;
	}
	if (!is_parent)
		clean_exit(cmd->bin, EXIT_SUCCESS);
}
