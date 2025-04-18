/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:18:31 by secros            #+#    #+#             */
/*   Updated: 2025/03/21 14:59:33 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BT_MALLOC_H
# define BT_MALLOC_H

# include <stdlib.h>

typedef struct s_sink
{
	void			*content;
	void			(*f)(void *);
	struct s_sink	*next;
}	t_sink;

//utils
t_sink	**get_sink(t_sink **bin);

//create
void	*fill_dishwasher(void *pt, void (*free_pt)(void *), t_sink **head);
void	*new_plate(size_t size, t_sink **bin);

//destroy
void	do_dishes(t_sink **head);
void	hand_wash(void *pt, t_sink **head);
void	remove_plate(void *pt, t_sink **head);

#endif