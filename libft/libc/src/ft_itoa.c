/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:57:23 by halnuma           #+#    #+#             */
/*   Updated: 2024/11/13 11:04:53 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_malloc_size(int n)
{
	int		div;
	int		size;

	div = n;
	size = 0;
	if (n <= 0)
	{
		size++;
		div *= -1;
	}
	while (div > 0)
	{
		size++;
		div /= 10;
	}
	return (size);
}

char	*ret_int_min(void)
{
	char	*int_min;
	char	*res;
	int		i;

	res = malloc(sizeof(char) * 12);
	if (!res)
		return (NULL);
	i = 0;
	int_min = "-2147483648";
	while (i < 13)
	{
		res[i] = int_min[i];
		i++;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		div;
	int		size;
	int		i;

	if (n == -2147483648)
		return (ret_int_min());
	size = get_malloc_size(n);
	res = malloc(sizeof(char) * size + 1);
	if (!res)
		return (NULL);
	div = n;
	if (n < 0)
		div *= -1;
	i = size - 1;
	while (i >= 0)
	{
		res[i] = (div % 10) + '0';
		div /= 10;
		i--;
	}
	if (n < 0)
		res[0] = '-';
	res[size] = '\0';
	return (res);
}
