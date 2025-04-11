/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:57:33 by secros            #+#    #+#             */
/*   Updated: 2025/04/11 13:59:37 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_lsts(t_list **lst)
{
	int	j;

	j = 0;
	while (lst[j])
	{
		ft_printf("[%s]", (char *)lst[j]->content);
		lst[j] = lst[j]->next;
		if (!lst[j])
		{
			ft_printf("\n");
			j++;
		}
	}
}

void	print_lst(t_list *lst)
{
	while (lst)
	{
		ft_printf("[%s]", lst->content);
		lst = lst->next;
	}
	ft_printf("\n");
}
