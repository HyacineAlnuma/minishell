/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:45:07 by halnuma           #+#    #+#             */
/*   Updated: 2024/11/11 10:39:52 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s) - 1;
	if ((char)c == '\0')
		return ((void *)&(s[ft_strlen(s)]));
	while (i >= 0)
	{
		if ((unsigned char)s[i] == (char)c)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
