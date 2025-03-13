/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:10:47 by secros            #+#    #+#             */
/*   Updated: 2025/03/13 17:37:32 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_the_mallocs(void **pt)
{
	size_t	i;

	i = 0;
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

int	lst_count_char(t_list *tokens, char c)
{
	int		count;
	char	search[2];

	count = 0;
	search[1] = '\0';
	search[0] = c;
	while (tokens)
	{
		if (tokens->content && !ft_strcmp((char *)tokens->content, search))
			count++;
		tokens = tokens->next;
	}
	return (count);
}
void	clear_to(t_list	*start, t_list *end)
{
	t_list	*tmp;
	t_list	*next;

	tmp = start->next;
	while (tmp && tmp != end)
	{
		next = tmp->next;
		ft_lstdelone(tmp, free);
		tmp = next;
	}
	start->next = end;
}

int	is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	is_redir(char c)
{
	if (c == '>' || c == '<' || c =='|')
		return (1);
	return (0);
}
