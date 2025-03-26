/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:48:13 by secros            #+#    #+#             */
/*   Updated: 2025/03/26 11:12:13 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_error(void)
{
	write (2, "zsh: parse count near `|'\n", 27);
	return (FALSE);
}

void	finish_sentence(char **str, char c)
{
	char	*next;
	char	*quote;

	next = NULL;
	quote = NULL;
	while (!next)
	{
		if (c == '\'')
			next = readline("quote>");
		if (c == '"')
			next = readline("dquote>");
		if (c == '|')
			next = readline("pipe>");
		if (c == '"' || c == '\'')
			quote = ft_strappend(quote, "\n");
		quote = ft_strappend(quote, next);
		free(next);
	}
	*str = ft_strappend(*str, quote);
	free(quote);
}

int	pipe_check(char *str, size_t i)
{
	(void)str;
	(void)i;
	return (1);
}

int	is_charset(char c, char *charset)
{
	size_t	i;

	i = 0;
	while (charset[i])
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

char	*synthax_quote(char *str)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && is_charset(str[i], "\'\"|"))
			quote = str[i++];
		if (quote == '|' && !pipe_check(str, i))
			return (NULL);
		while (str[i] && ((quote == '|' && (!ft_isalnum(str[i])))
				|| (is_charset(quote, "'\"") && str[i] != quote)))
			i++;
		if ((str[i] == quote && is_charset(quote, "'\"")) || (quote == '|' && ft_isalnum(str[i])))
			quote = 0;
		if (!str[i] && quote)
		{
			finish_sentence(&str, quote);
			i++;
		}
		if (!quote)
			i++;
	}
	return (str);
}
