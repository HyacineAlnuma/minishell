/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:55:19 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/11 10:28:41 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size;
	char	*res;

	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	res = ft_calloc(sizeof(char), size);
	if (!res)
		return (NULL);
	ft_strlcat(res, s1, size);
	ft_strlcat(res, s2, size);
	return (res);
}

char	*ft_strappend(char *s1, char *s2)
{
	char	*fs;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	while (s1 && s1[len1])
		len1++;
	while (s2 && s2[len2])
		len2++;
	fs = (char *) malloc(sizeof(char) * (len1 + len2 + 1));
	if (!fs && s1)
		free(s1);
	if (!fs)
		return (NULL);
	fs[len1 + len2] = '\0';
	while ((len2-- > 0))
		fs[len1 + len2] = s2[len2];
	while (len1-- > 0)
		fs[len1] = s1[len1];
	if (s1)
		free(s1);
	return (fs);
}
