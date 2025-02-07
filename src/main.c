/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/02/07 13:31:03 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>
#include <stdio.h>
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

static void	free_the_mallocs(void **pt)
{
	size_t	i;

	i = 0;
	if (!pt)
		return ;
	while (pt[i])
	{
		free(pt[i]);
		pt[i++] = NULL;
	}
	free(pt);
	pt = NULL;
}

/* static t_com	*alloc_command(t_com **previous)
{
	t_com	*current;
	size_t	i;

	if (!previous)
	{
		current = ft_calloc(sizeof(t_com *), 2);
		if (!current)
			return (NULL);
		current[0] = ft_calloc(sizeof(t_com), 1);
			if (!current[0])
				return (free(current), NULL);
		return (current);
	}
	i = 0;
	while (previous[i])
		i++;
	current = ft_calloc(sizeof(t_com *), i + 2);
	if (!current)
		return (NULL);
	current[i] = ft_calloc(sizeof(t_com), 1);
	if (!current[i])
		return (free_the_mallocs((void **) previous));
	ft_memcpy(current, previous, (sizeof(t_com *) * i));
	return (free(previous), current);
}
 */

char	**remove_first(char **tab)
{
	size_t	j;
	char **new_tab;

	new_tab = ft_calloc(sizeof(char *), (ft_tablen(tab)));
	if (!new_tab)
		return (NULL);
	j = 1;
	while (tab[j])
	{
		new_tab[j - 1] = tab[j];
		j++;
	}
	return (new_tab);
}

void	set_struct(char ***tab, t_com **command)
{
	size_t	i;

	i = 0;
	while (command[i])
	{
		command[i]->command = tab[i][0];
		command[i]->option = remove_first(tab[i]); // Malloc so fail possible errno ?
		i++;
	}
}

t_com	**create_struct(char ***tab, size_t count)
{
	t_com	**command;
	size_t	i;

	i = 0;
	command = ft_calloc(sizeof(t_com *), (count + 1));
	if (!command)
		return (NULL);
	while (i < count)
	{
		command[i] = malloc(sizeof(t_com));
		if (!command[i++])
		{
			free_the_mallocs((void **) command);
			return (NULL);
		}
	}
	set_struct(tab, command);
	return (command);
}

static int	synthax_error(char *str)
{
	size_t	j;
	int	i;

	j = 0;
	i = 0;
	while (str[j])
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
			return (write (2, "zsh: parse count near `|'", 26), 1);
	}
	if (i == 1)
		return (2);
	return (0);
}

char ***divide_arg(char *str)
{
	char ***command;
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
	return (command);
}

t_com	**parsing(char *str)
{
	size_t	count;
	t_com	**command;
	char	*tmp;

	count = synthax_error(str);
	if (!str || count == 1)
		return (free(str), NULL);
	else if (count == 2)
	{
		tmp = readline("pipe> ");
		str = ft_strappend(str, tmp);
		free(tmp);
		return (parsing(str));
	}
	count = count_char(str, '|');
	command = create_struct(divide_arg(str), count + 1);
	return (command);
}

int	main(int ac, char **av)
{
	char	*input;
	t_com	**command;
	int		i;
	int		j;

	(void) ac;
	(void) av;
	command = NULL;
	while (1)
	{	
		i = 0;
		input = readline("Prompt :");
		command = parsing(input);
		free(input);
		if (!command)
			return (0);
		while (command[i])
		{
			ft_printf("COM : [%s] ", command[i]->command);
			j = 0;
			while (command[i]->option[j])
			{
				ft_printf("OPTION %d : [%s], ", j, command[i]->option[j]);
				j++;
			}
			i++;
			ft_printf("|PIPE|");
		}
		ft_printf("\n");
	}
}
