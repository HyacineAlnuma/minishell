/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:56:27 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/19 11:37:11 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**create_final_array(const char *str, char c)
{
	size_t		i;
	int			parts;
	int			last_sep;
	char		**res;

	last_sep = 0;
	parts = 0;
	i = 0;
	while (i <= ft_strlen(str))
	{
		if (str[i] == c || i == ft_strlen(str))
		{
			if ((i - last_sep) > 0)
				parts++;
			last_sep = i + 1;
		}
		i++;
	}
	res = malloc(sizeof(char *) * (parts + 1));
	if (!res)
		return (NULL);
	return (res);
}

char	*ft_strndup(const char *src, int size)
{
	char	*dest;
	int		i;

	dest = (char *)malloc((sizeof(char) * size) + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static void	*destroy_split(char **res, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (NULL);
}

static void	*set_res(char **res, const char *s, char c)
{
	size_t		i;
	size_t		j;
	size_t		last_sep;

	i = 0;
	j = -1;
	last_sep = 0;
	while (i <= ft_strlen(s))
	{
		if (s[i] == c || i == ft_strlen(s))
		{
			if ((i - last_sep) > 0)
			{
				res[++j] = ft_strndup((char *)&s[last_sep], (i - last_sep));
				if (!res[j])
					return (destroy_split(res, j));
			}
			last_sep = i + 1;
		}
		i++;
	}
	res[++j] = NULL;
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;

	res = create_final_array(s, c);
	if (!res)
		return (NULL);
	res = set_res(res, s, c);
	if (!res)
		return (NULL);
	return (res);
}
