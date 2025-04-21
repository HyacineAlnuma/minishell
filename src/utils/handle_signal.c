/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:01:37 by secros            #+#    #+#             */
/*   Updated: 2025/04/21 10:19:58 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		last_status_code(130, 2);
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		if (!g_sigint_flag || g_sigint_flag == 2)
			rl_redisplay();
		g_sigint_flag = 2;
	}
	return ;
}

void	sig_handler_hd(int signum)
{
	if (signum == SIGINT)
	{
		close(0);
		g_sigint_flag = 1;
	}
}
