/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/03/13 11:58:10 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "color.h"

/* 
	create the struct
		(first element of lst) = 1
		handle redirection (and heredoc)
			set token to infile / outfile
		if first element token = command else token = option
		if token = pipe OK
			new_struct and repeat with the other lst OK
	when lst = null
		parsing end
		destroy the lst but keep content; lst_clear with NULL
		send strut to exect

*/

char	*remove_quote(char *str)
{
	char	*tmp;

	if (!str)
		return (NULL);
	if (str[0] == '\'' || str[0] == '"')
	{
		tmp = ft_substr(str, 1, ft_strlen(str) - 2);
		free(str);
		if (tmp[0] == '\0')
		{
			free(tmp);
			tmp = NULL;
		}
		str = tmp;
	}
	return (str);
}

int	env_handling(t_list *tokens, t_list **env)
{
	char	*token;

	while (tokens)
	{
		token = (char *)tokens->content;
		if (token && token[0] != '\'')
			tokens->content = handle_env(token, env);
		tokens = tokens->next;
	}
	return (1);
}


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
	if (tmp)
		ft_lstdelone(tmp, free);
}

t_list	**cut_instruction(t_list *tokens)
{
	t_list	**pipe;
	t_exec	**com;
	int		count;
	int		i;

	i = 0;
	count = count_pipe(tokens);
	pipe = ft_calloc(sizeof(t_list *), (count + 2));
	if (!pipe)
		return (NULL);
	com = ft_calloc(sizeof(t_exec *), (count + 2));
	if (!com)
		return (NULL);
	while (i <= count)
	{
		pipe[i++] = tokens;
		cut_pipe(&tokens);
	}
	return (pipe);
}

t_list	*parsing(char *str, t_list **env)
{
	t_list	**piped;
	t_list	*tokens;
	
	tokens = create_token_list(str);
	env_handling(tokens, env);
	piped = cut_instruction(tokens);
	/*	Next step is to change environnement handling to iter over each element of the list OK
		cut lst based on pipe OK
		create t_doc tab
		then remove quote tokens OK
		then merge token into final token OK
	*/
	int		j = 0;
	while (piped[j])
	{
		ft_printf("[%s]", (char *)piped[j]->content);
		piped[j] = piped[j]->next;
		if (!piped[j])
		{
			ft_printf("\n");
			j++;
		}
	}
	return (NULL);
}
