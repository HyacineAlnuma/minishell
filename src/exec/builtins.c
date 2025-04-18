/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:32:53 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/18 10:48:41 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_exec *cmd)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	while (cmd->opt[i] && cmd->opt[i][0] == '-')
	{
		j = 1;
		while(cmd->opt[i][j] == 'n')
			j++;
		if (cmd->opt[i][j] != '\0')
			break ;
		i++;
	}
	k = i;
	while (cmd->opt[k])
	{
		ft_printf("%s", cmd->opt[k]);
		if (cmd->opt[k + 1])
			ft_printf(" ");
		k++;
	}
	if (i == 1)
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
				ptr->content = fill_dishwasher(ft_strjoin("PWD=", pwd), \
				free, get_sink(NULL));
			if (!ft_strncmp(ptr->content, "OLDPWD=", 7))
				ptr->content = fill_dishwasher(ft_strjoin("OLDPWD=", oldpwd), \
				free, get_sink(NULL));
			ptr = ptr->next;
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cmd->opt[1], 2);
		ft_putendl_fd(": No such file or directory", 2);
		last_status_code(1, 2);
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

int	check_export_arg(char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
	{
		print_error("export", arg, ": not a valid identifier");
		return (0);
	}
	while (arg[++i])
	{
		if (arg[i] == '=')
			break ;
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			print_error("export", arg, ": not a valid identifier");
			return (0);
		}
	}
	return (1);
}

void	export(t_exec *cmd, t_list **env, int is_parent)
{
	t_list	*new_line;
	char	*env_line;

	if (!cmd->opt[1] && !is_parent)
	{
		print_exp_env(env);
		clean_exit(cmd->bin, EXIT_SUCCESS);
	}
	if (cmd->opt[1] && is_parent)
	{
		if (!check_export_arg(cmd->opt[1]))
			return ;
		unset(cmd, env);
		env_line = fill_dishwasher(ft_strdup(cmd->opt[1]), \
		free, get_sink(NULL));
		new_line = fill_dishwasher(ft_lstnew(env_line), free, get_sink(NULL));
		if (!new_line || !new_line)
		{
			perror("malloc error");
			return ;
		}
		ft_lstadd_back(env, new_line);
	}
	if (!is_parent)
		clean_exit(cmd->bin, EXIT_SUCCESS);
}

void	unset(t_exec *cmd, t_list **env)
{
	char	*data_ref;
	t_list	*ptr;
	int		var_size;

	if (!cmd->opt[1])
		return ;
	var_size = 0;
	while (cmd->opt[1][var_size] && cmd->opt[1][var_size] != '=')
		var_size++;
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
		ft_lst_remove_if(env, data_ref, ft_strcmp, 0);
}
