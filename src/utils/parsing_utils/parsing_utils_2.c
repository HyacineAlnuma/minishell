/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:59:00 by secros            #+#    #+#             */
/*   Updated: 2025/04/23 11:34:09 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quote(char *str, t_sink *bin)
{
	char	*tmp;

	if (!str)
		return (NULL);
	tmp = str;
	if (str[0] == '\'' || str[0] == '"')
	{
		tmp = fill_dishwasher(ft_substr(str, 1, \
ft_strlen(str) - 2), free, &bin);
		if (tmp[0] == '\0')
		{
			hand_wash(tmp, &bin);
			return (NULL);
		}
	}
	return (tmp);
}

int	check_heredoc(t_list **head)
{
	if (!strncmp((*head)->content, "<<\0", 3))
	{
		if ((*head)->next && (*head)->next->next)
		{
			*head = (*head)->next->next;
			return (0);
		}
	}
	return (1);
}

int	is_quote(char c)
{
	if (c == '"' || c == '\'')
		return (1);
	return (0);
}

int	env_handling(t_list *tokens, t_list **env, t_sink *bin)
{
	char	*token;
	char	*n_tok;
	char	*new_str;
	int		i;

	while (tokens)
	{
		token = (char *)tokens->content;
		if (tokens->next)
			n_tok = (char *)tokens->next->content;
		else
			n_tok = NULL;
		if (token && token[0] != '\'' && check_heredoc(&tokens))
		{
			new_str = handle_env(token, env, bin);
			if (!new_str)
				return (0);
			i = ft_strlen(new_str);
			if (new_str[i - 1] == '$' && n_tok && (is_quote(n_tok[0])))
				new_str[i - 1] = '\0';
			tokens->content = new_str;
		}
		tokens = tokens->next;
	}
	return (1);
}

int	compare(char *str, char *str_ref)
{
	if (!str || !str_ref)
	{
		if (!str && !str_ref)
			return (0);
		else
			return (1);
	}
	return (ft_strncmp(str, str_ref, ft_strlen(str_ref)));
}
