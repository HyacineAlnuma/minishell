/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ascii.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:56 by secros            #+#    #+#             */
/*   Updated: 2025/02/14 11:12:48 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ascii_bis(void)
{
	ft_printf("\x1B[35m%s", ASCII22);
	ft_printf("\x1B[35m%s", ASCII23);
	ft_printf("\x1B[35m%s", ASCII24);
	ft_printf("\x1B[31m%s", ASCII25);
	ft_printf("\x1B[31m%s", ASCII26);
	ft_printf("\x1B[31m%s", ASCII27);
	ft_printf("\x1B[31m%s", ASCII28);
	ft_printf("\x1B[31m%s", ASCII29);
	ft_printf("\x1B[31m%s", ASCII30);
	ft_printf("\x1B[31m%s", ASCII31);
	ft_printf("\x1B[31m%s", ASCII32);
	ft_printf("\x1B[31m%s", ASCII33);
	ft_printf("\x1B[31m%s", ASCII34);
	ft_printf("\x1B[31m%s", ASCII35);
	ft_printf("\n\n");
	ft_printf("\x1B[37m");
}

void	print_ascii(void)
{
	ft_printf("\n\n");
	ft_printf("\x1B[34m%s", ASCII1);
	ft_printf("\x1B[34m%s", ASCII2);
	ft_printf("\x1B[34m%s", ASCII3);
	ft_printf("\x1B[34m%s", ASCII4);
	ft_printf("\x1B[34m%s", ASCII5);
	ft_printf("\x1B[34m%s", ASCII6);
	ft_printf("\x1B[36m%s", ASCII7);
	ft_printf("\x1B[36m%s", ASCII8);
	ft_printf("\x1B[36m%s", ASCII9);
	ft_printf("\x1B[36m%s", ASCII10);
	ft_printf("\x1B[36m%s", ASCII11);
	ft_printf("\x1B[36m%s", ASCII12);
	ft_printf("\x1B[36m%s", ASCII13);
	ft_printf("\x1B[36m%s", ASCII14);
	ft_printf("\x1B[35m%s", ASCII15);
	ft_printf("\x1B[35m%s", ASCII16);
	ft_printf("\x1B[35m%s", ASCII17);
	ft_printf("\x1B[35m%s", ASCII18);
	ft_printf("\x1B[35m%s", ASCII19);
	ft_printf("\x1B[35m%s", ASCII20);
	ft_printf("\x1B[35m%s", ASCII21);
	print_ascii_bis();
}
