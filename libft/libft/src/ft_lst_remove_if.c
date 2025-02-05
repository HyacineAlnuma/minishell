/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_remove_if.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:16:10 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/05 11:23:22 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
	t_list	*tmp;

	if ((*begin_list == NULL) || (begin_list == NULL))
		return ;
	if (cmp((*begin_list)->content, data_ref) == 0x0)
	{
		tmp = *begin_list;
		*begin_list = (*begin_list)->next;
		free(tmp);
		ft_lst_remove_if(begin_list, data_ref, cmp);
	}
	else
		ft_lst_remove_if(&((*begin_list)->next), data_ref, cmp);
}
