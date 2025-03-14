/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:23:16 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/03 14:47:05 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

int	ft_printptr(size_t n, char *base)
{
	if (!n)
	{
		ft_printstr("(nil)");
		return (5);
	}
	else
	{
		ft_printstr("0x");
		ft_putnbr_base(n, base);
		return (ft_ptrlen(n, base, 0) + 2);
	}
}

void	ft_putnbr_base(ssize_t n, char *base)
{
	size_t	base_len;

	base_len = ft_strlen(base);
	if (n < 0)
	{
		ft_printchar('-');
		ft_putnbr_base(n * -1, base);
	}
	else
	{
		if ((size_t) n >= base_len)
		{
			ft_putnbr_base((n / base_len), base);
			ft_printchar(base[n % base_len]);
		}
		else
		{
			ft_printchar(base[n % base_len]);
		}
	}
}

int	ft_ptrlen(ssize_t n, char *base, int count)
{
	size_t	base_len;

	base_len = ft_strlen(base);
	if (n < 0)
	{
		count++;
		return (ft_ptrlen(n * -1, base, count));
	}
	else
	{
		if ((size_t) n >= base_len)
		{
			count++;
			return (ft_ptrlen((n / base_len), base, count));
		}
		else
		{
			count++;
			return (count);
		}
	}
}
