/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:10:47 by secros            #+#    #+#             */
/*   Updated: 2025/04/11 14:05:13 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_the_mallocs(void *alloc)
{
	size_t	i;
	void	**pt;

	i = 0;
	pt = alloc;
	if (!pt)
		return ;
	while (pt[i])
	{
		free(pt[i]);
		pt[i++] = NULL;
	}
	free(pt);
	pt = NULL;
}

int	is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	is_redir(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	return (0);
}

void	skip_space(char *str, size_t *i)
{
	while (is_space(str[*i]))
		*i += 1;
}

void	*add_empty(t_list **lst)
{
	t_list	*new;

	new = ft_lstnew(NULL);
	if (!new)
		return (NULL);
	ft_lstadd_back(lst, new);
	return (new);
}
