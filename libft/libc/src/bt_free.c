/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:42:38 by secros            #+#    #+#             */
/*   Updated: 2025/04/28 14:38:23 by secros           ###   ########.fr       */
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
