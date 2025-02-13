/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ascii.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:56 by secros            #+#    #+#             */
/*   Updated: 2025/02/13 10:18:00 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ascii_bis(void)
{
	printf("\x1B[35m%s", ASCII22);
	printf("\x1B[35m%s", ASCII23);
	printf("\x1B[35m%s", ASCII24);
	printf("\x1B[31m%s", ASCII25);
	printf("\x1B[31m%s", ASCII26);
	printf("\x1B[31m%s", ASCII27);
	printf("\x1B[31m%s", ASCII28);
	printf("\x1B[31m%s", ASCII29);
	printf("\x1B[31m%s", ASCII30);
	printf("\x1B[31m%s", ASCII31);
	printf("\x1B[31m%s", ASCII32);
	printf("\x1B[31m%s", ASCII33);
	printf("\x1B[31m%s", ASCII34);
	printf("\x1B[31m%s", ASCII35);
	printf("\n\n");
	printf("\x1B[37m");
}

void	print_ascii(void)
{
	printf("\n\n");
	printf("\x1B[34m%s", ASCII1);
	printf("\x1B[34m%s", ASCII2);
	printf("\x1B[34m%s", ASCII3);
	printf("\x1B[34m%s", ASCII4);
	printf("\x1B[34m%s", ASCII5);
	printf("\x1B[34m%s", ASCII6);
	printf("\x1B[36m%s", ASCII7);
	printf("\x1B[36m%s", ASCII8);
	printf("\x1B[36m%s", ASCII9);
	printf("\x1B[36m%s", ASCII10);
	printf("\x1B[36m%s", ASCII11);
	printf("\x1B[36m%s", ASCII12);
	printf("\x1B[36m%s", ASCII13);
	printf("\x1B[36m%s", ASCII14);
	printf("\x1B[35m%s", ASCII15);
	printf("\x1B[35m%s", ASCII16);
	printf("\x1B[35m%s", ASCII17);
	printf("\x1B[35m%s", ASCII18);
	printf("\x1B[35m%s", ASCII19);
	printf("\x1B[35m%s", ASCII20);
	printf("\x1B[35m%s", ASCII21);
	print_ascii_bis();
}
