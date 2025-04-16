/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:45:00 by secros            #+#    #+#             */
/*   Updated: 2025/04/16 19:20:13 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_sigint_flag;

void	print_prompt(t_list **env)
{
	char	*prompt;

	prompt = find_node(env, "$USER");
	if (!prompt)
		prompt = find_user_in_pwd();
	ft_printf("%s%s%s ", BG_RED, prompt, RESET);
	prompt = find_node(env, "$PWD");
	ft_printf("%s%s%s%s\n", BOLD, FG_BRIGHT_BLUE, prompt, RESET);
}

int	synthax_errror(char *str)
{
	size_t	i;
	char	error;

	i = 0;
	error = 1;
	while (str[i])
	{
		while (is_space(str[i]))
			i++;
		if (str[i] == '|' && error == 1)
			return (1);
	}
	return (0);
}

void	minishell(char **envp, t_list **env)
{
	char	*input;
	t_exec	**command;
	t_sink	*bin;

	command = NULL;
	bin = NULL;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		g_sigint_flag = 0;
		print_prompt(env);
		input = fill_dishwasher(readline("minishell % "), free, &bin);
		g_sigint_flag = 1;
		if (!input)
			break ;
		add_history(input);
		command = parsing(input, env, &bin);
		if (command)
			exec(command, env, envp);
		do_dishes(&bin);
	}
	printf("exit\n");
}

int	main(int ac, char **av, char **envp)
{
	char	*mini_str;
	t_list	**env;

	(void) av;
	if (ac != 1)
	{
		write (2, "Error: Bad arguments\n", 21);
		return (1);
	}
	env = lst_env(envp);
	if (!isatty(STDIN_FILENO))
	{
		mini_str = fill_dishwasher(get_next_line(0), free, get_sink(NULL));
		if (!mini_str)
			return (2);
		exec(parsing(mini_str, env, get_sink(NULL)), env, envp);
		return (do_dishes(get_sink(NULL)), 0);
	}
	print_ascii();
	minishell(envp, env);
	rl_clear_history();
	do_dishes(get_sink(NULL));
}
