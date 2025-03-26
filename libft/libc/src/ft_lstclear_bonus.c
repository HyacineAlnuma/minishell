/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:45:06 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/17 11:44:00 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*ptr;
	t_list	*temp;

	if (!(*lst))
		return ;
	ptr = *lst;
	while (ptr)
	{
		temp = ptr->next;
		ft_lstdelone(ptr, del);
		ptr = temp;
	}
	*lst = NULL;
}
