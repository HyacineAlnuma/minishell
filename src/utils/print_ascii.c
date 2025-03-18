/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ascii.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:56 by secros            #+#    #+#             */
/*   Updated: 2025/03/18 09:37:04 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ascii_bis(void)
{
	ft_printf("%s%s", FG_BLUE, ASCII22);
	ft_printf("%s%s", FG_BLUE, ASCII23);
	ft_printf("%s%s", FG_BLUE, ASCII24);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII25);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII26);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII27);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII28);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII29);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII30);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII31);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII32);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII33);
	ft_printf("%s%s", FG_BRIGHT_MAGENTA, ASCII34);
	ft_printf("%s%s%s", FG_BRIGHT_MAGENTA, ASCII35, RESET);
	ft_printf("\n\n");
}

void	print_ascii(void)
{
	ft_printf("\n\n");
	ft_printf("%s%s", FG_BRIGHT_CYAN, ASCII1);
	ft_printf("%s%s", FG_BRIGHT_CYAN, ASCII2);
	ft_printf("%s%s", FG_BRIGHT_CYAN, ASCII3);
	ft_printf("%s%s", FG_BRIGHT_CYAN, ASCII4);
	ft_printf("%s%s", FG_BRIGHT_CYAN, ASCII5);
	ft_printf("%s%s", FG_BRIGHT_CYAN, ASCII6);
	ft_printf("%s%s", FG_CYAN, ASCII7);
	ft_printf("%s%s", FG_CYAN, ASCII8);
	ft_printf("%s%s", FG_CYAN, ASCII9);
	ft_printf("%s%s", FG_CYAN, ASCII10);
	ft_printf("%s%s", FG_CYAN, ASCII11);
	ft_printf("%s%s", FG_CYAN, ASCII12);
	ft_printf("%s%s", FG_CYAN, ASCII13);
	ft_printf("%s%s", FG_CYAN, ASCII14);
	ft_printf("%s%s", FG_BLUE, ASCII15);
	ft_printf("%s%s", FG_BLUE, ASCII16);
	ft_printf("%s%s", FG_BLUE, ASCII17);
	ft_printf("%s%s", FG_BLUE, ASCII18);
	ft_printf("%s%s", FG_BLUE, ASCII19);
	ft_printf("%s%s", FG_BLUE, ASCII20);
	ft_printf("%s%s", FG_BLUE, ASCII21);
	print_ascii_bis();
}
