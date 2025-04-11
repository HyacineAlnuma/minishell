/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:29:37 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/11 11:26:31 by halnuma          ###   ########.fr       */
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
				ft_putstr_fd("minishell: exit:", 2);
				ft_putstr_fd(cmd->opt[1], 2);
				ft_putendl_fd(": numeric argument required", 2);
				clean_exit(cmd->bin, 2);
			}
		}
		exit_code = (unsigned char)ft_atoi(cmd->opt[1]);
	}
	else
		exit_code = 0;
	return (exit_code);
}
