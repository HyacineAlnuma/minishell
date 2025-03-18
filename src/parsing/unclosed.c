/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:48:13 by secros            #+#    #+#             */
/*   Updated: 2025/03/18 13:15:30 by secros           ###   ########.fr       */
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
	while (!next || !ft_strchr(next, c))
	{
		free(next);
		if (c == '\'')
			next = readline("quote>");
		if (c == '"')
			next = readline("dquote>");
		if (c == '|')
			next = readline("pipe>");
		if (c == '"' || c == '\'')
			quote = ft_strappend(quote, "\n");
		quote = ft_strappend(quote, next);
	}
	*str = ft_strappend(*str, quote);
	free(quote);
}

int	pipe_check(char *str, size_t i, char *c)
{
	size_t	j;

	j = i;
	while (str[j])
	{
		if (!(str[j] == ' ' || (str[j] >= 9 && str[j] <= 13)))
			break ;
		if (j == 0 || str[j] == '|')
			return (pipe_error());
		j--;
	}
	while (str[i])
	{
		if (!(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
			return (TRUE);
		if (str[i] == '|')
			return (pipe_error());
		i++;
	}
	finish_sentence(&str, *c);
	return (1);
}

char	*synthax_quote(char *str)
{
	size_t	i;
	char	c;

	i = 0;
	c = 0;
	while (str && str[i])
	{
		c = 0;
		if (c == 0 && (str[i] == '"' || str[i] == '\'' || str[i] == '|'))
			c = str[i++];
		if (c == '|' && !pipe_check(str, i, &c))
			return (NULL);
		if ((c == '\'' || c == '"') && !ft_strchr(&str[i], c))
			finish_sentence(&str, c);
		while (str[i] != c && (c != 0 || c == '|'))
			i++;
		i++;
	}
	return (str);
}
