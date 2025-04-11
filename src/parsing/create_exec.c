/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:57:54 by secros            #+#    #+#             */
/*   Updated: 2025/04/11 10:46:28 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cut_pipe(t_list **tokens)
{
	t_list	*prev;

	prev = NULL;
	while (*tokens)
	{
		if ((*tokens)->content && !strcmp((char *)(*tokens)->content, "|"))
			break ;
		prev = *tokens;
		*tokens = (*tokens)->next;
	}
	if (*tokens)
	{
		(*tokens) = (*tokens)->next;
		if (prev)
			prev->next = NULL;
	}
}

int	check_pipe(t_list **pipe, int count)
{
	int	i;

	i = 0;
	while (i <= count)
	{
		while (1)
		{
			if (!pipe[i])
			{
				write(2, "hell: syntax error near unexpected token `|'\n", 46);
				return (1);
			}
			if (pipe[i]->content)
				break ;
			pipe[i] = pipe[i]->next;
		}
		i++;
	}
	return (0);
}

t_list	**cut_instruction(t_list *tokens, int count)
{
	t_list	**pipe;
	int		i;

	i = 0;
	pipe = ft_calloc(sizeof(t_list *), (count + 2));
	if (!pipe)
		return (NULL);
	if (count == 0)
	{
		*pipe = tokens;
		return (pipe);
	}
	while (i <= count)
	{
		pipe[i++] = tokens;
		cut_pipe(&tokens);
	}
	if (check_pipe(pipe, count))
		return (NULL);
	return (pipe);
}
