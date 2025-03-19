/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:34:14 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/19 09:31:02 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_program(t_exec **cmds, t_list **env)
{
	(void)cmds;
	(void)env;
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

void	print_env(t_list **env)
{
	t_list	*ptr;

	ptr = *env;
	while (ptr)
	{
		ft_printf("%s\n", ptr->content);
		ptr = ptr->next;
	}
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

void	pwd(void)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_printf("%s\n", pwd);
	else
		perror("getcwd() error");
	exit(EXIT_SUCCESS);
}

int	exec_builtins(t_exec *cmd, t_list **env)
{
	if (!ft_strncmp(cmd->cmd, "echo", 5))
		echo(cmd);
	else if (!ft_strncmp(cmd->cmd, "cd", 3))
		exit(EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "pwd", 4))
		pwd();
	else if (!ft_strncmp(cmd->cmd, "export", 7))
		exit(EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "unset", 6))
		exit(EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "env", 4))
		print_env(env);
	else if (!ft_strncmp(cmd->cmd, "exit", 5))
		exit(EXIT_SUCCESS);
	return (0);
}
