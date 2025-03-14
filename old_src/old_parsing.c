/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:43:08 by secros            #+#    #+#             */
/*   Updated: 2025/03/14 15:10:55 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec	**parsing(char *str, t_list **env)
{
	size_t	count;
	t_exec	**command;
	char	*tmp;

	count = synthax_error(str);
	if (!str || count == 1)
		return (free(str), NULL);
	else if (count == 2)
	{
		tmp = readline("pipe> ");
		str = ft_strappend(str, tmp);
		free(tmp);
		return (parsing(str, env));
	}
	add_history(str);
	count = count_char(str, '|');
	str = handle_env(str, env);
	command = create_struct(divide_arg(str), count + 1);
	return (command);
}

char	***divide_arg(char *str)
{
	char	***command;
	char	**args;
	size_t	tablen;

	args = ft_split(str, '|');
	if (!args)
		return (NULL);
	tablen = ft_tablen(args);
	command = ft_calloc(sizeof(char **), tablen + 1);
	if (!command)
	{
		free_the_mallocs((void **) args);
		return (NULL);
	}
	while (tablen--)
	{
		command[tablen] = ft_multi_split(args[tablen], CHARSET);
		if (!command[tablen])
			free_the_mallocs((void **) command);
	}
	free_the_mallocs((void **) args);
	free(str);
	return (command);
}

static int	synthax_error(char *str)
{
	size_t	j;
	int		i;

	j = 0;
	i = 0;
	while (str && str[j])
	{
		i = 1;
		while (str[j] && str[j] != '|')
		{
			if (!(str[j] == ' ' || (str[j] >= 9 && str[j] <= 13)))
				i = 0;
			j++;
		}
		if (str[j] && str[j] == '|')
		{
			j++;
			i++;
		}
		if (i > 1)
			return (write (2, "zsh: parse count near `|'\n", 27), 1);
	}
	if (i == 1)
		return (2);
	return (0);
}
