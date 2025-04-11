/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:38:20 by secros            #+#    #+#             */
/*   Updated: 2025/04/11 16:53:27 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	count_token(char *str, size_t *i, size_t *count, char *quote)
{
	while (str[*i + *count] && (!is_space(str[*i + *count]) || *quote))
	{
		if (str[*i + *count] == *quote || (!*quote && is_redir(str[*i])))
		{
			*count += 1;
			break ;
		}
		else if (!*quote && (is_redir(str[*i + *count]) \
		|| str[*i + *count] == '"' || str[*i + *count] == '\''))
			break ;
		*count += 1;
	}
	return ;
}

char	*split_token(char *str, size_t *i, t_sink **bin)
{
	size_t		count;
	char		*token;
	char		quote;

	count = 0;
	quote = 0;
	token = NULL;
	skip_space(str, i);
	if (str[*i] == '"' || str[*i] == '\'')
		quote = str[*i + count++];
	count_token(str, i, &count, &quote);
	token = fill_dishwasher(ft_substr(str, *i, count), free, bin);
	*i += count;
	return (token);
}

char	*unify_token(char *str, char **token, t_sink **bin, size_t	*i)
{
	while (str[*i] && is_redir(str[*i]) && is_redir(str[*i - 1]))
	{
		if (str[*i] == '|')
			break ;
		*token = fill_dishwasher(ft_strjoin(*token, \
split_token(str, i, bin)), free, bin);
		if (!*token)
			return (NULL);
	}
	return (*token);
}

t_list	*create_token_list(char *str, t_sink **bin)
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
		if (!token)
			return (NULL);
		if (!unify_token(str, &token, bin, &i))
			return (NULL);
		new = fill_dishwasher(ft_lstnew(token), free, bin);
		if (!new)
			return (NULL);
		if (token[0] != '\0')
			ft_lstadd_back(&tokens, new);
		if (str[i] && (is_space(str[i]) || (is_redir(str[i]) && \
		!is_redir(str[i - 1])) || (is_redir(str[i - 1]) && !is_redir(str[i]))))
			if (!fill_dishwasher(add_empty(&tokens), free, bin))
				return (NULL);
	}
	return (tokens);
}
