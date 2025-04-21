/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:34:14 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/21 10:58:43 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	manage_exit_args(char **opt)
{
	int		i;
	t_ull	exit_code;

	i = 0;
	if (!opt[1])
		return (0);
	if (!all_digit(opt[1]))
		return (2);
	else
		if (opt[2])
			return (1);
	if (ft_strlen(opt[1]) > 18)
		return (2);
	else
	{
		exit_code = ft_atoull(opt[1]);
		if (exit_code > LLONG_MAX)
			return (2);
	}
	return ((unsigned char)exit_code);
}

void	exit_program(t_exec *cmd, int is_parent)
{
	unsigned char	exit_code;

	exit_code = manage_exit_args(cmd->opt);
	if (exit_code == 2 && (is_parent || !cmd->nb_cmd))
		print_error("exit", cmd->opt[1], ": numeric argument required");
	else if (exit_code == 1 && (is_parent || !cmd->nb_cmd))
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		if (is_parent)
			return ;
	}
	rl_clear_history();
	if (cmd->bin)
		do_dishes(get_sink(cmd->bin));
	if (is_parent)
		printf("exit\n");
	clean_exit(NULL, exit_code);
}

void	print_env(t_exec *cmd, t_list **env)
{
	t_list	*ptr;

	ptr = *env;
	while (ptr)
	{
		if (ft_strchr(ptr->content, '='))
			ft_printf("%s\n", ptr->content);
		ptr = ptr->next;
	}
	clean_exit(cmd->bin, EXIT_SUCCESS);
}

void	pwd(t_exec *cmd)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_printf("%s\n", pwd);
	else
		perror("getcwd() error");
	clean_exit(cmd->bin, EXIT_SUCCESS);
}

int	exec_builtins(t_exec *cmd, t_list **env)
{
	if (!ft_strncmp(cmd->cmd, "echo", 5))
		echo(cmd);
	else if (!ft_strncmp(cmd->cmd, "cd", 3))
		clean_exit(cmd->bin, EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "pwd", 4))
		pwd(cmd);
	else if (!ft_strncmp(cmd->cmd, "export", 7))
		export(cmd, env, 0);
	else if (!ft_strncmp(cmd->cmd, "unset", 6))
		clean_exit(cmd->bin, EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "env", 4))
		print_env(cmd, env);
	else if (!ft_strncmp(cmd->cmd, "exit", 5))
		exit_program(cmd, 0);
	return (0);
}
