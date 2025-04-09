/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:34:14 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/09 14:46:39 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_program(t_exec *cmd)
{
	t_sink	**ptr;
	int		i;

	do_dishes(get_sink(NULL));
	if (cmd->bin)
		do_dishes(cmd->bin);
	rl_clear_history();
	ptr = get_sink(NULL);
	i = 0;
	while (*ptr)
	{
		i++;
		*ptr = (*ptr)->next;
	}
	exit(EXIT_SUCCESS);
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
	do_dishes(get_sink(NULL));
	do_dishes(cmd->bin);
	exit(EXIT_SUCCESS);
}

void	pwd(t_exec *cmd)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_printf("%s\n", pwd);
	else
		perror("getcwd() error");
	do_dishes(get_sink(NULL));
	do_dishes(cmd->bin);
	exit(EXIT_SUCCESS);
}

int	exec_builtins_bis(t_exec *cmd, t_list **env)
{
	if (!ft_strncmp(cmd->cmd, "unset", 6))
	{
		do_dishes(get_sink(NULL));
		do_dishes(cmd->bin);
		exit(EXIT_SUCCESS);
	}
	else if (!ft_strncmp(cmd->cmd, "env", 4))
		print_env(cmd, env);
	else if (!ft_strncmp(cmd->cmd, "exit", 5))
	{
		do_dishes(get_sink(NULL));
		do_dishes(cmd->bin);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

int	exec_builtins(t_exec *cmd, t_list **env)
{
	if (!ft_strncmp(cmd->cmd, "echo", 5))
		echo(cmd);
	else if (!ft_strncmp(cmd->cmd, "cd", 3))
	{
		do_dishes(get_sink(NULL));
		do_dishes(cmd->bin);
		exit(EXIT_SUCCESS);
	}
	else if (!ft_strncmp(cmd->cmd, "pwd", 4))
		pwd(cmd);
	else if (!ft_strncmp(cmd->cmd, "export", 7))
	{
		do_dishes(get_sink(NULL));
		do_dishes(cmd->bin);
		exit(EXIT_SUCCESS);
	}
	return (exec_builtins(cmd, env));
}
