/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hd_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:02:19 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/21 10:17:32 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	synthax_expand(char *str, int i)
{
	if (!(str[i] == '$' && str[i + 1] == '('))
		return (2);
	while (str[i] && str[i] != ')')
		i++;
	if (!str[i])
	{
		ft_putendl_fd("minishell: can't find matching `)'", 2);
		return (0);
	}
	return (1);
}

enum e_doc	find_type(char *str)
{
	if (!strncmp(str, "<\0", 2))
		return (INFILE);
	else if (!strncmp(str, "<<\0", 3))
		return (HEREDOC);
	else if (!strncmp(str, ">\0", 2))
		return (OUTFILE);
	else if (!strncmp(str, ">>\0", 3))
		return (APPEND);
	ft_printf("hell: syntax error near unexpected token `%c'\n", \
str[ft_strlen(str) - 1]);
	return (-1);
}
