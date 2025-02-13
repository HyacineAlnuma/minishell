/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:09:51 by secros            #+#    #+#             */
/*   Updated: 2025/02/13 11:18:54 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_struct(char ***tab, t_exec **command)
{
	size_t	i;

	i = 0;
	while (command[i])
	{
		command[i]->cmd = tab[i][0];
		command[i]->opt = tab[i];
		i++;
	}
}

t_exec	**create_struct(char ***tab, size_t count)
{
	t_exec	**command;
	size_t	i;

	i = 0;
	if (!tab || !tab[0])
		return (NULL);
	command = ft_calloc(sizeof(t_exec *), (count + 1));
	if (!command)
		return (NULL);
	while (i < count)
	{
		command[i] = malloc(sizeof(t_exec));
		if (!command[i++])
		{
			free_the_mallocs((void **) command);
			return (NULL);
		}
	}
	set_struct(tab, command);
	return (command);
}
