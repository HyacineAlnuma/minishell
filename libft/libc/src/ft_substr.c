/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:39:02 by halnuma           #+#    #+#             */
/*   Updated: 2024/11/11 17:45:45 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_size(char const *s, unsigned int start, size_t len)
{
	size_t	size;

	if (start >= ft_strlen(s))
		size = 1;
	else if ((ft_strlen(s) - start) > len)
		size = len + 1;
	else
		size = ft_strlen(s) - start + 1;
	return (size);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*res;
	size_t	size;

	size = get_size(s, start, len);
	res = malloc(sizeof(char) * size);
	if (!res)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		res[0] = '\0';
		return (res);
	}
	i = start;
	j = 0;
	while (s[i] && j < len)
	{
		res[j] = s[i];
		i++;
		j++;
	}
	res[j] = '\0';
	return (res);
}
