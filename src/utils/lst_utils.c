/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:00:23 by secros            #+#    #+#             */
/*   Updated: 2025/04/11 14:04:17 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lst_len(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

char	**convert_lst_in_tab(t_list *lst, t_sink *bin)
{
	char	**tab;
	size_t	nb_tok;

	nb_tok = lst_len(lst);
	tab = new_plate(sizeof(char *) * (nb_tok + 1), &bin);
	if (!tab)
		return (NULL);
	tab[nb_tok] = NULL;
	nb_tok = 0;
	while (lst)
	{
		tab[nb_tok++] = lst->content;
		lst = lst->next;
	}
	return (tab);
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
		if (tokens->content && *(char *)tokens->content != '\0'
			&& !ft_strcmp((char *)tokens->content, search))
		{
			count++;
		}
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
		tmp = next;
	}
	start->next = end;
}

void	merge_all(t_list *lst, t_sink *bin)
{
	while (lst)
	{
		merge_tokens(lst, bin);
		lst = lst->next;
	}
}
