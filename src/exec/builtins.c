/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:32:53 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/11 11:32:04 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_exec *cmd)
{
	int	i;

	i = 1;
	while (!ft_strncmp(cmd->opt[i], "-n", 3))
		i++;
	while (cmd->opt[i])
	{
		ft_printf("%s", cmd->opt[i]);
		if (cmd->opt[i + 1])
			ft_printf(" ");
		i++;
	}
	if (ft_strncmp(cmd->opt[1], "-n", 3))
		ft_printf("\n");
	clean_exit(cmd->bin, EXIT_SUCCESS);
}

void	change_dir(t_exec *cmd, t_list *ptr, char *oldpwd)
{
	char	pwd[PATH_MAX];

	if (!chdir(cmd->opt[1]))
	{
		if (!getcwd(pwd, sizeof(pwd)))
			perror("getcwd() error");
		while (ptr)
		{
			if (!ft_strncmp(ptr->content, "PWD=", 4))
				ptr->content = ft_strjoin("PWD=", pwd);
			if (!ft_strncmp(ptr->content, "OLDPWD=", 7))
				ptr->content = ft_strjoin("OLDPWD=", oldpwd);
			ptr = ptr->next;
		}
	}
}

void	cd(t_exec *cmd, t_list **env)
{
	t_list	*ptr;
	char	oldpwd[PATH_MAX];
	char	*previouspwd;

	if (!cmd->opt[1])
		return ;
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		perror("getcwd() error");
	ptr = *env;
	previouspwd = get_previous_pwd(env);
	if (!ft_strncmp(cmd->opt[1], "-", 2))
	{
		if (!previouspwd)
			printf("minishell: cd: OLDPWD not set\n");
		cmd->opt[1] = previouspwd;
	}
	change_dir(cmd, ptr, oldpwd);
}

void	export(t_exec *cmd, t_list **env)
{
	t_list	*new_line;
	char	*env_line;

	if (!cmd->opt[1])
	{
		print_exp_env(env);
		return ;
	}
	unset(cmd, env);
	env_line = fill_dishwasher(ft_strdup(cmd->opt[1]), free, get_sink(NULL));
	new_line = fill_dishwasher(ft_lstnew(env_line), free, get_sink(NULL));
	if (!new_line || !new_line)
	{
		perror("malloc error");
		return ;
	}
	ft_lstadd_back(env, new_line);
}

void	unset(t_exec *cmd, t_list **env)
{
	char	*data_ref;
	t_list	*ptr;
	int		var_size;

	var_size = 0;
	while (cmd->opt[1][var_size] && cmd->opt[1][var_size] != '=')
		var_size++;
	ptr = *env;
	data_ref = NULL;
	while (ptr)
	{
		if (!ft_strncmp(ptr->content, cmd->opt[1], var_size))
		{
			data_ref = ptr->content;
			break ;
		}
		ptr = ptr->next;
	}
	if (data_ref)
		ft_lst_remove_if(env, data_ref, ft_strcmp);
}
