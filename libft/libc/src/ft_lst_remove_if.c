/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_remove_if.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:16:10 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/14 10:41:49 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_remove_if(t_list **l, void *data_ref, int (*cmp)(), int do_free)
{
	t_list	*tmp;

	if ((*l == NULL) || (l == NULL))
		return ;
	if (cmp((*l)->content, data_ref) == 0)
	{
		tmp = *l;
		*l = (*l)->next;
		if (do_free)
			free(tmp);
		ft_lst_remove_if(l, data_ref, cmp, do_free);
	}
	else
		ft_lst_remove_if(&((*l)->next), data_ref, cmp, do_free);
}
