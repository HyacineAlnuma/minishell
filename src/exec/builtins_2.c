/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:34:14 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/16 11:51:02 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	manage_exit_args(char **opt)
{
	int	i;
	int	j;
	ull exit_code;

	i = 0;
	if (!opt[1])
		return (0);
	while(opt[++i])
	{
		j = -1;
		while (opt[i][++j])
			if (opt[i][j] && ft_isalpha(opt[i][j]))
				return (2);
	}
	if (i > 2)
		return (1);
	if (ft_strlen(opt[1]) > 18)
		return (2);
	else
	{
		exit_code = ft_atoull(opt[1]);
		if (exit_code > 9223372036854775807)
			return (2);
	}
	return ((unsigned char)exit_code);
}

void	exit_program(t_exec *cmd)
{
	unsigned char	exit_code;

	exit_code = manage_exit_args(cmd->opt);
	if (exit_code == 2)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->opt[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
	}
	else if (exit_code == 1)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return ;
	}
	rl_clear_history();
	if (cmd->bin)
		do_dishes(get_sink(cmd->bin));
	clean_exit(NULL, exit_code);
}

void	print_env(t_exec *cmd, t_list **env)
{
	t_list	*ptr;

	ptr = *env;
	while (ptr)
	{
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
	signal(SIGPIPE, SIG_IGN);
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
		clean_exit(cmd->bin, EXIT_SUCCESS);
	return (0);
}
