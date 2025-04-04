/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:38:20 by secros            #+#    #+#             */
/*   Updated: 2025/03/21 13:54:39 by secros           ###   ########.fr       */
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

int	merge_tokens(t_list *tokens, t_sink *bin)
{
	t_list	*tmp;
	char	*new_token;

	tmp = tokens;
	new_token = NULL;
	if (!tokens->content)
		return (0);
	while (tmp && tmp->content)
	{
		tmp->content = remove_quote((char *)tmp->content, bin);
		new_token = fill_dishwasher(ft_strjoin(new_token, \
		(char *)tmp->content), free, &bin);
		tmp = tmp->next;
	}
	tokens->content = new_token;
	clear_to(tokens, tmp);
	return (1);
}

char	*split_token(char *str, size_t *i, t_sink *bin)
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
	while (str[*i + count] && (!is_space(str[*i + count]) || quote))
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
	token = fill_dishwasher(ft_substr(str, *i, count), free, &bin);
	*i += count;
	return (token);
}

t_list	*create_token_list(char *str, t_sink *bin)
{
	size_t	i;
	t_list	*tokens;
	t_list	*new;
	char	*token;

	i = 0;
	tokens = NULL;
	while (str[i])
	{
		token = split_token(str, &i, bin);
		while (str[i] && is_redir(str[i]) && is_redir(str[i - 1]))
			token = fill_dishwasher(ft_strjoin(token, \
			split_token(str, &i, bin)), free, &bin);
		if (token[0] == '\0')
			return (tokens);
		new = fill_dishwasher(ft_lstnew(token), free, &bin);
		if (!token || !new)
			return (ft_lstclear(&tokens, free), NULL);
		ft_lstadd_back(&tokens, new);
		if (str[i] && (is_space(str[i]) || (is_redir(str[i]) && \
		!is_redir(str[i - 1])) || (is_redir(str[i - 1]) && !is_redir(str[i]))))
			fill_dishwasher(add_empty(&tokens), free, &bin);
	}
	return (tokens);
}
