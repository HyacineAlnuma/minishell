/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:29:37 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/18 17:36:17 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_exit(t_sink **bin, int exit_code)
{
	if (bin)
		do_dishes(get_sink(bin));
	exit(exit_code);
}

int	check_exit_arg(t_exec *cmd)
{
	int	i;
	int	exit_code;

	if (cmd->opt[1] && *cmd->opt[1])
	{
		i = -1;
		while (cmd->opt[1][++i])
		{
			if (!ft_isdigit(cmd->opt[1][i]))
			{
				print_error("exit", cmd->opt[1], ": numeric argument required");
				clean_exit(cmd->bin, 2);
			}
		}
		exit_code = (unsigned char)ft_atoi(cmd->opt[1]);
	}
	else
		exit_code = 0;
	return (exit_code);
}

void	print_error(char *cmd, char *arg, char *message)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (message)
		ft_putendl_fd(message, 2);
	else
		ft_putstr_fd("\n", 2);
}

void	close_temp_file(t_exec **cmds)
{
	int	i;
	int	j;

	i = 0;
	while (cmds[i])
	{
		if (cmds[i]->here_doc)
		{
			i++;
			continue ;
		}
		j = 0;
		while (cmds[i]->docs[j])
		{
			if (cmds[i]->docs[j]->type >= HEREDOC)
			{
				close(cmds[i]->docs[j]->type);
				unlink(HD_TEMP_FILE);
			}
			j++;
		}
		i++;
	}
}
