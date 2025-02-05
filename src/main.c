/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/02/05 15:50:27 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void	free_the_mallocs(void **pt)
{
	size_t	i;

	i = 0;
	if (!pt)
		return ;
	while (pt[i])
		free(pt[i++]);
	free(pt);
}

t_com	**alloc_command(t_com **previous)
{
	t_com	**current;
	size_t	i;

	if (!previous)
	{
		current = ft_calloc(sizeof(t_com **), 2);
		if (!current)
			return (NULL);
		current[0] = ft_calloc(sizeof(t_com), 1);
			if (!current[0])
				return (free(current), NULL);
		return (current);
	}
	i = 0;
	while (previous[i])
		i++;
	current = ft_calloc(sizeof(t_com **), i + 2);
	if (!current)
		return (NULL);
	current[i] = ft_calloc(sizeof(t_com), 1);
	if (!current[i])
		return (free_the_mallocs((void**) current), NULL);
	ft_memcpy(current, previous, (sizeof(t_com *) * i));
	return (free(previous), current);
}

t_com	**parsing(char *str)
{
	size_t	i;
	char	**args;
	t_com	**current;

	i = 0;
	current = NULL;
	
	args = ft_multi_split(str, CHARSET);
	current = alloc_command(current);
	current = alloc_command(current);
	if (!str || !str[i])
		return (NULL);
	
	return (current);
}

int	main(int ac, char **av)
{
	char	*input;

	(void) ac;
	(void) av;
	while (1)
	{	
		input = readline("Prompt :");
		if (parsing(input))
			printf("Something\n");
		else
			printf("\n");
	}
}
