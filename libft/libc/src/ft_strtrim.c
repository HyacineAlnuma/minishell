/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:17:16 by halnuma           #+#    #+#             */
/*   Updated: 2024/11/11 18:05:03 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_str(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		i;
	int		j;
	int		k;
	int		size;

	i = 0;
	j = ft_strlen(s1) - 1;
	k = 0;
	while (is_in_str(set, (unsigned char)s1[i]))
		i++;
	while (is_in_str(set, (unsigned char)s1[j]) && j > i)
		j--;
	size = j - i + 2;
	res = malloc(sizeof(char) * size);
	if (!res)
		return (NULL);
	while (i <= j)
	{
		res[k] = s1[i];
		i++;
		k++;
	}
	res[k] = '\0';
	return (res);
}
