/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:36:42 by secros            #+#    #+#             */
/*   Updated: 2025/04/18 17:41:51 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lst_hand_wash_if(t_list **begin_list, void *data_ref, \
int (*cmp)(), t_sink *bin)
{
	t_list	*tmp;

	if ((*begin_list == NULL) || (begin_list == NULL))
		return ;
	if (cmp((*begin_list)->content, data_ref) == 0)
	{
		tmp = *begin_list;
		*begin_list = (*begin_list)->next;
		hand_wash(tmp, &bin);
		ft_lst_hand_wash_if(begin_list, data_ref, cmp, bin);
	}
	else
		ft_lst_hand_wash_if(&((*begin_list)->next), data_ref, cmp, bin);
}

int	all_digit(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return ((i != 0));
}
