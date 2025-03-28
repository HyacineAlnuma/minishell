/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:42:38 by secros            #+#    #+#             */
/*   Updated: 2025/03/28 13:40:44 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bt_malloc.h"

void	do_dishes(t_sink **head)
{
	t_sink	*tmp;
	t_sink	*tmp_next;

	tmp = *head;
	while (tmp)
	{
		tmp_next = tmp->next;
		if (tmp->f)
			tmp->f(tmp->content);
		free(tmp);
		tmp = tmp_next;
	}
	*head = NULL;
}

void	hand_wash(void *pt, t_sink **head)
{
	t_sink	*prev;
	t_sink	*tmp;

	if (!head || !*head)
		return (free(pt));
	tmp = *head;
	prev = NULL;
	while (tmp)
	{
		if (tmp->content == pt)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*head = tmp->next;
			if (tmp->f)
				tmp->f(tmp->content);
			return (free(tmp));
		}
		prev = tmp;
		tmp = tmp->next;
	}
	free(pt);
}
void	remove_plate(void *pt, t_sink **head)
{
	t_sink	*prev;
	t_sink	*tmp;

	if (!head || !*head)
		return ;
	tmp = *head;
	prev = NULL;
	while (tmp)
	{
		if (tmp->content == pt)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*head = tmp->next;
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
