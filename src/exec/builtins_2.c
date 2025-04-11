/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:34:14 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/11 11:31:40 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_program(t_exec *cmd)
{
	unsigned char	exit_code;

	rl_clear_history();
	exit_code = check_exit_arg(cmd);
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
	if (!ft_strncmp(cmd->cmd, "echo", 5))
		echo(cmd);
	else if (!ft_strncmp(cmd->cmd, "cd", 3))
		clean_exit(cmd->bin, EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "pwd", 4))
		pwd(cmd);
	else if (!ft_strncmp(cmd->cmd, "export", 7))
		clean_exit(cmd->bin, EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "unset", 6))
		clean_exit(cmd->bin, EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "env", 4))
		print_env(cmd, env);
	else if (!ft_strncmp(cmd->cmd, "exit", 5))
		clean_exit(cmd->bin, EXIT_SUCCESS);
	return (0);
}
