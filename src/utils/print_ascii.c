/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ascii.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:56 by secros            #+#    #+#             */
/*   Updated: 2025/03/19 14:13:09 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ascii_bis(void)
{
	ft_printf("%s%s", FG_MAGENTA, ASCII22);
	ft_printf("%s%s", FG_MAGENTA, ASCII23);
	ft_printf("%s%s", FG_MAGENTA, ASCII24);
	ft_printf("%s%s", FG_RED, ASCII25);
	ft_printf("%s%s", FG_RED, ASCII26);
	ft_printf("%s%s", FG_RED, ASCII27);
	ft_printf("%s%s", FG_RED, ASCII28);
	ft_printf("%s%s", FG_RED, ASCII29);
	ft_printf("%s%s", FG_RED, ASCII30);
	ft_printf("%s%s", FG_RED, ASCII31);
	ft_printf("%s%s", FG_RED, ASCII32);
	ft_printf("%s%s", FG_RED, ASCII33);
	ft_printf("%s%s", FG_RED, ASCII34);
	ft_printf("%s%s%s", FG_RED, ASCII35, RESET);
	ft_printf("\n\n");
}

void	print_ascii(void)
{
	ft_printf("\n\n");
	ft_printf("%s%s", FG_BLUE, ASCII1);
	ft_printf("%s%s", FG_BLUE, ASCII2);
	ft_printf("%s%s", FG_BLUE, ASCII3);
	ft_printf("%s%s", FG_BLUE, ASCII4);
	ft_printf("%s%s", FG_BLUE, ASCII5);
	ft_printf("%s%s", FG_BLUE, ASCII6);
	ft_printf("%s%s", FG_CYAN, ASCII7);
	ft_printf("%s%s", FG_CYAN, ASCII8);
	ft_printf("%s%s", FG_CYAN, ASCII9);
	ft_printf("%s%s", FG_CYAN, ASCII10);
	ft_printf("%s%s", FG_CYAN, ASCII11);
	ft_printf("%s%s", FG_CYAN, ASCII12);
	ft_printf("%s%s", FG_CYAN, ASCII13);
	ft_printf("%s%s", FG_CYAN, ASCII14);
	ft_printf("%s%s", FG_MAGENTA, ASCII15);
	ft_printf("%s%s", FG_MAGENTA, ASCII16);
	ft_printf("%s%s", FG_MAGENTA, ASCII17);
	ft_printf("%s%s", FG_MAGENTA, ASCII18);
	ft_printf("%s%s", FG_MAGENTA, ASCII19);
	ft_printf("%s%s", FG_MAGENTA, ASCII20);
	ft_printf("%s%s", FG_MAGENTA, ASCII21);
	print_ascii_bis();
}
