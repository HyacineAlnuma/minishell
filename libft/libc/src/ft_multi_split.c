/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multi_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:16:13 by secros            #+#    #+#             */
/*   Updated: 2025/02/05 14:26:58 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_charset(char c, char *set)
{
	size_t	i;

	i = 0;
	while (set && set[i])
		if (set[i++] == c)
			return (1);
	return (0);
}

static size_t	countsplit(const char *s, char *c)
{
	size_t	i;
	size_t	word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if ((i == 0 || is_charset(s[i - 1], c)) && !is_charset(s[i], c))
			word++;
		i++;
	}
	return (word);
}

static size_t	to_next_c(const char *s, char *c, size_t index)
{
	size_t	i;

	i = 0;
	while (!is_charset(s[index], c) && s[index])
	{
		i++;
		index++;
	}
	return (i);
}

char	**ft_multi_split(char const *s, char *c)
{
	size_t	i;
	size_t	i2;
	char	**fs;

	i = 0;
	i2 = 0;
	fs = (char **) ft_calloc(sizeof(char *), (countsplit(s, c) + 1));
	if (!fs)
		return (NULL);
	while (s[i])
	{
		if ((i == 0 || is_charset(s[i - 1], c)) && !is_charset(s[i], c))
		{
			fs[i2] = ft_substr(s, i, to_next_c(s, c, i));
			if (!fs[i2])
			{
				ft_free_split(fs);
				return (NULL);
			}
			i2++;
		}
		i++;
	}
	return (fs);
}
