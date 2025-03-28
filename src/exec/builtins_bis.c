/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:34:14 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/28 15:43:37 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_program(t_exec *cmd)
{
	do_dishes(get_sink(NULL));
	do_dishes(cmd->bin);
	rl_clear_history();
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

char	*get_previous_pwd(t_list **env)
{
	char	*previouspwd;
	char	*str;
	int		i;
	t_list	*ptr;

	ptr = *env;
	previouspwd = NULL;
	while (ptr)
	{
		if (!ft_strncmp(ptr->content, "OLDPWD=", 7))
		{
			i = -1;
			str = ptr->content;
			while (str[++i])
			{
				if (str[i] == '=')
					break ;
			}
			previouspwd = &str[++i];
		}
		ptr = ptr->next;
	}
	return (previouspwd);
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
	else if (!ft_strncmp(cmd->cmd, "unset", 6))
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
