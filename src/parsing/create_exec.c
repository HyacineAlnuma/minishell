/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:57:54 by secros            #+#    #+#             */
/*   Updated: 2025/03/18 13:14:11 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cut_pipe(t_list **tokens)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = NULL;
	while (*tokens)
	{
		if ((*tokens)->content && !strcmp((char *)(*tokens)->content, "|"))
		{
			tmp = *tokens;
			break ;
		}
		prev = *tokens;
		*tokens = (*tokens)->next;
	}
	if (*tokens)
	{
		(*tokens) = (*tokens)->next;
		prev->next = NULL;
	}
}

t_list	**cut_instruction(t_list *tokens, int count)
{
	t_list	**pipe;
	int		i;

	i = 0;
	pipe = ft_calloc(sizeof(t_list *), (count + 2));
	if (!pipe)
		return (NULL);
	while (i <= count)
	{
		pipe[i++] = tokens;
		cut_pipe(&tokens);
	}
	return (pipe);
}
