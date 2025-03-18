w/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   better_mallocs.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:18:31 by secros            #+#    #+#             */
/*   Updated: 2025/03/17 16:21:37 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BETTER_MALLOCS_H
# define BETTER_MALLOCS_H

typedef struct s_garb
{
	void			*content;
	void			(*f)(void *);
	struct s_garb	*next;
}	t_garb;

//create
void	*add_garbage(void *pt, void (*free_pt)(void *), t_garb **head);
void	*ft_malloc(size_t size, t_garb **bin);

//destroy
void	ft_free(void *pt, t_garb **head);
void 	clear_garbage(t_garb **head);

#endif