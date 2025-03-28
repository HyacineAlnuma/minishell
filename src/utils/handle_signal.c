/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:01:37 by secros            #+#    #+#             */
/*   Updated: 2025/03/28 11:30:35 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		// g_sigint_flag = 2;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		if (!g_sigint_flag)
			rl_redisplay();
	}
	return ;
}
