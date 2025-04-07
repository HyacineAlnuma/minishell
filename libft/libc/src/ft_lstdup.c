/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:58:19 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/28 15:34:37 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	**ft_lstdup(t_list **lst, void (*del)(void *))
{
	int		lst_size;
	t_list	*ptr;
	t_list	**dup;
	t_list	*new;

	lst_size = ft_lstsize(*lst);
	dup = (t_list **)malloc(sizeof(t_list **));
	*dup = NULL;
	ptr = *lst;
	if (!dup)
		return (NULL);
	while (ptr)
	{
		new = ft_lstnew(ptr->content);
		if (!new)
		{
			ft_lstclear(dup, del);
			return (NULL);
		}
		ft_lstadd_back(dup, new);
		ptr = ptr->next;
	}
	return (dup);
}
