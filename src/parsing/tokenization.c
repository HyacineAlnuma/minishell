/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:38:20 by secros            #+#    #+#             */
/*   Updated: 2025/03/13 11:58:18 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*add_empty(t_list **lst)
{
	t_list	*new;

	new = ft_lstnew(NULL);
	if (!new)
		return (NULL);
	ft_lstadd_back(lst, new);
	return (new);
}

int	merge_tokens(t_list **tokens)
{
	t_list	*tmp;
	t_list	*prev;
	char	*new_token;

	tmp = *tokens;
	while (tmp)
	{
		new_token = NULL;
		prev = tmp;
		while (tmp && tmp->content)
		{
			tmp->content = remove_quote((char *)tmp->content);
			new_token = ft_strappend(new_token, (char *)tmp->content);
			tmp = tmp->next;
		}
		while (tmp && !tmp->content)
			tmp = tmp->next;
		prev->content = new_token;
		clear_to(prev, tmp);
	}
	return (1);
}

char	*split_token(char *str, size_t *i)
{
	unsigned int	count;
	char			*token;
	char			quote;

	count = 0;
	quote = 0;
	while (is_space(str[*i]))
		*i += 1;
	if (str[*i] == '"' || str[*i] == '\'')
		quote = str[*i + count++];
	while(str[*i + count] && (!is_space(str[*i + count]) || quote))
	{
		if (str[*i + count] == quote || (!quote && is_redir(str[*i])))
		{
			count++;
			break ;
		}
		else if (!quote && (is_redir(str[*i + count]) \
		|| str[*i + count] == '"' || str[*i + count] == '\''))
			break ;
		count++;
	}
	token = ft_substr(str, *i, count);
	*i += count;
	return (token);
}

t_list	*create_token_list(char *str)
{
	size_t	i;
	t_list	*tokens;
	t_list	*new;
	char	*token;

	i = 0;
	tokens = NULL;
	while (str[i])
	{
		token = split_token(str, &i);
		while(str[i] && is_redir(str[i]) && is_redir(str[i - 1]))
			token = ft_strappend(token, split_token(str, &i));
		new = ft_lstnew(token);
		if (!token || !new)
			return (ft_lstclear(&tokens, free), NULL);
		ft_lstadd_back(&tokens, new);
		if (str[i] && (is_space(str[i]) || (is_redir(str[i]) && \
		!is_redir(str[i - 1])) || (is_redir(str[i - 1]) && !is_redir(str[i]))))
			add_empty(&tokens);
	}
	return (tokens);
}
