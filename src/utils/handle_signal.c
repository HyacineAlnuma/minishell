/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:01:37 by secros            #+#    #+#             */
/*   Updated: 2025/03/19 11:43:31 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_sigint_flag = 2;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		if (!g_sigint_flag)
			rl_redisplay();
	}
	return ;
}
