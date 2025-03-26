/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:18:31 by secros            #+#    #+#             */
/*   Updated: 2025/03/26 10:42:48 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BT_MALLOC_H
# define BT_MALLOC_H

# include <stdlib.h>

/**
 * @brief Node of chained list
 * @param *content Pointer to an element
 * @param (*f) Function to destroy this element (can be null)
 * @param *next Pointer to next node
 * 
 */
typedef struct s_sink	t_sink;

struct s_sink
{
	void			*content;
	void			(*f)(void *);
	struct s_sink	*next;
};

//utils
/**
 * @brief Get the sink pointer (static)
 * 
 * @param bin Possible garbage head (added to the static bin)
 * @return t_sink** address of static pointer
 */
t_sink	**get_sink(t_sink **bin);

//create
/**
 * @brief Add a allocated element to the sink
 * 
 * @param pt Pointer to an allocated element
 * @param free_pt Function to destroy pt
 * @param head Sink pointer
 * @return pt if success NULL if failure
 */
void	*fill_dishwasher(void *pt, void (*free_pt)(void *), t_sink **head);
/**
 * @brief malloc and add to sink with *(f) = free (if !bin just alloc)
 * 
 * @param size Size of malloc
 * @param bin Sink head pointer
 * @return pt or NULL on failure
 */
void	*new_plate(size_t size, t_sink **bin);

//destroy
/**
 * @brief Remove a element of the sink (Unfree)
 * 
 * (do nothing if pt is not found in sink)
 * 
 * @param pt Pointer to an element
 * @param head Sink pointer
 */
void	remove_plate(void *pt, t_sink **head);
/**
 * @brief Clear the sink by applaying each free_funct to content
 * 
 * @param head Sink head pointer
 */
void	do_dishes(t_sink **head);
/**
 * @brief apply free_funct to an element and remove it from sink.
 * 
 *  if !head or pt not in sink behave like free
 * 
 * @param pt Pointer to an element
 * @param head Sink head pointer
 */
void	hand_wash(void *pt, t_sink **head);

#endif