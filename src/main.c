/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:45:00 by secros            #+#    #+#             */
/*   Updated: 2025/03/28 15:40:47 by secros           ###   ########.fr       */
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

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_list	**env;
	t_exec	**command;
	t_sink	*bin;

	(void) av;
	bin = NULL;
	if (ac != 1)
	{
		write (2, "Error: Bad arguments\n", 21);
		fflush(stderr);
		return (1);
	}
	print_ascii();
	command = NULL;
	env = lst_env(envp);
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
		input = synthax_quote(input);
		command = parsing(input, env, &bin);
		if (command)
			exec(command, env, envp);
		do_dishes(&bin);
	}
	do_dishes(get_sink(NULL));
}
