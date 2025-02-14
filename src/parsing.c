/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/02/14 11:13:53 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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

/* static t_exec	*alloc_command(t_exec **previous)
{
	t_exec	*current;
	size_t	i;

	if (!previous)
	{
		current = ft_calloc(sizeof(t_exec *), 2);
		if (!current)
			return (NULL);
		current[0] = ft_calloc(sizeof(t_exec), 1);
			if (!current[0])
				return (free(current), NULL);
		return (current);
	}
	i = 0;
	while (previous[i])
		i++;
	current = ft_calloc(sizeof(t_exec *), i + 2);
	if (!current)
		return (NULL);
	current[i] = ft_calloc(sizeof(t_exec), 1);
	if (!current[i])
		return (free_the_mallocs((void **) previous));
	ft_memcpy(current, previous, (sizeof(t_exec *) * i));
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

void	set_struct(char ***tab, t_exec **command)
{
	size_t	i;

	i = 0;
	while (command[i])
	{
		command[i]->cmd = tab[i][0];
		command[i]->opt = tab[i]; // Malloc so fail possible errno ?
		i++;
	}
}

t_exec	**create_struct(char ***tab, size_t count)
{
	t_exec	**command;
	size_t	i;

	i = 0;
	command = ft_calloc(sizeof(t_exec *), (count + 1));
	if (!command)
		return (NULL);
	while (i < count)
	{
		command[i] = malloc(sizeof(t_exec));
		if (!command[i++])
		{
			free_the_mallocs((void **) command);
			return (NULL);
		}
	}
	set_struct(tab, command);
	//ft_free_split(*tab);
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
	free(str);
	return (command);
}

t_exec	**parsing(char *str)
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
		str = ft_strjoin(str, tmp);
		free(tmp);
		return (parsing(str));
	}
	add_history(str);
	count = count_char(str, '|');
	command = create_struct(divide_arg(str), count + 1);
	return (command);
}

t_list	**lst_env(char **envp)
{
	int		i;
	t_list	**lst_env;
	t_list	*new_line;

	i = 0;
	lst_env = (t_list **)ft_calloc(sizeof(t_list **), 1);
	if (!lst_env)
		return (NULL);
	while (envp[i])
	{
		new_line = ft_lstnew(envp[i]);
		if (!new_line)
			return (NULL);
		ft_lstadd_back(lst_env, new_line);
		i++;
	}
	return (lst_env);
}

void	print_ascii_bis(void)
{
	printf("\x1B[35m%s", ASCII22);
	printf("\x1B[35m%s", ASCII23);
	printf("\x1B[35m%s", ASCII24);
	printf("\x1B[31m%s", ASCII25);
	printf("\x1B[31m%s", ASCII26);
	printf("\x1B[31m%s", ASCII27);
	printf("\x1B[31m%s", ASCII28);
	printf("\x1B[31m%s", ASCII29);
	printf("\x1B[31m%s", ASCII30);
	printf("\x1B[31m%s", ASCII31);
	printf("\x1B[31m%s", ASCII32);
	printf("\x1B[31m%s", ASCII33);
	printf("\x1B[31m%s", ASCII34);
	printf("\x1B[31m%s", ASCII35);
	printf("\n\n");
	printf("\x1B[37m");
}

void	print_ascii(void)
{
	printf("\n\n");
	printf("\x1B[34m%s", ASCII1);
	printf("\x1B[34m%s", ASCII2);
	printf("\x1B[34m%s", ASCII3);
	printf("\x1B[34m%s", ASCII4);
	printf("\x1B[34m%s", ASCII5);
	printf("\x1B[34m%s", ASCII6);
	printf("\x1B[36m%s", ASCII7);
	printf("\x1B[36m%s", ASCII8);
	printf("\x1B[36m%s", ASCII9);
	printf("\x1B[36m%s", ASCII10);
	printf("\x1B[36m%s", ASCII11);
	printf("\x1B[36m%s", ASCII12);
	printf("\x1B[36m%s", ASCII13);
	printf("\x1B[36m%s", ASCII14);
	printf("\x1B[35m%s", ASCII15);
	printf("\x1B[35m%s", ASCII16);
	printf("\x1B[35m%s", ASCII17);
	printf("\x1B[35m%s", ASCII18);
	printf("\x1B[35m%s", ASCII19);
	printf("\x1B[35m%s", ASCII20);
	printf("\x1B[35m%s", ASCII21);
	print_ascii_bis();
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_list	**env;
	t_exec	**command;
	// int		i;
	// int		j;

	(void) ac;
	(void) av;
	print_ascii();
	command = NULL;
	env = lst_env(envp);
	while (1)
	{
		//i = 0;
		input = readline("Prompt :");
		command = parsing(input);
		//free(input);
		if (!command)
			return (0);
		// while (command[i])
		// {
		// 	ft_printf("COM : [%s] ", command[i]->cmd);
		// 	j = 0;
		// 	while (command[i]->opt[j])
		// 	{
		// 		ft_printf("opt %d : [%s], ", j, command[i]->opt[j]);
		// 		j++;
		// 	}
		// 	i++;
		// 	ft_printf("|PIPE|");
		// }
		// ft_printf("\n");
		exec(command, envp, env);
	}
}
