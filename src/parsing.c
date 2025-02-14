/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/02/14 11:11:17 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "color.h"

static int	synthax_error(char *str)
{
	size_t	j;
	int		i;

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

void	print_prompt(t_list **env)
{
	char	*prompt;

	prompt = find_node(env, "$USER");
	ft_printf("%s%s%s ", BG_BLUE, prompt, RESET);
	prompt = find_node(env, "$PWD");
	ft_printf("%s%s%s%s\n", BOLD, FG_BRIGHT_GREEN, prompt, RESET);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_list	**env;
	t_exec	**command;

	(void) av;
	if (ac != 1)
	{
		write (2, "Error\nBad arguments\n", 20);
		return (1);
	}
	print_ascii();
	command = NULL;
	env = lst_env(envp);
	while (1)
	{
		print_prompt(env);
		input = readline("Minishell % ");
		command = parsing(input, env);
		exec(command, env, envp);
	}
}
