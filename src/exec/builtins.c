/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:32:53 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/18 16:25:51 by halnuma          ###   ########.fr       */
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
		while (cmd->opt[i][j] == 'n')
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

int	change_pwd_vars(t_list *ptr, char *oldpwd, char *pwd)
{
	if (!ft_strncmp(ptr->content, "PWD=", 4))
	{
		ptr->content = fill_dishwasher(ft_strjoin("PWD=", pwd), \
		free, get_sink(NULL));
		if (!ptr->content)
		{
			perror("Malloc error");
			return (0);
		}
	}
	if (!ft_strncmp(ptr->content, "OLDPWD=", 7))
	{
		ptr->content = fill_dishwasher(ft_strjoin("OLDPWD=", oldpwd), \
		free, get_sink(NULL));
		if (!ptr->content)
		{
			perror("Malloc error");
			return (0);
		}
	}
	return (1);
}

int	change_dir(t_exec *cmd, t_list *ptr, char *oldpwd)
{
	char	pwd[PATH_MAX];

	if (!chdir(cmd->opt[1]))
	{
		if (!getcwd(pwd, sizeof(pwd)))
			perror("getcwd() error");
		while (ptr)
		{
			if (!change_pwd_vars(ptr, oldpwd, pwd))
				return (0);
			ptr = ptr->next;
		}
	}
	else
	{
		print_error("cd", cmd->opt[1], ": No such file or directory");
		last_status_code(1, 2);
	}
	return (1);
}

void	cd(t_exec *cmd, t_list **env)
{
	t_list	*ptr;
	char	oldpwd[PATH_MAX];
	char	*previouspwd;

	if (!cmd->opt[1])
	{
		cmd->opt[1] = fill_dishwasher(
				ft_strjoin("/home/", find_user_in_pwd()), free, cmd->bin
				);
		if (!cmd->opt[1])
			return ;
	}
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
	if (!change_dir(cmd, ptr, oldpwd))
		return ;
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

void	export_vars(t_exec *cmd, t_list **env, int i)
{
	t_list	*new_line;
	char	*env_line;

	if (!check_export_arg(cmd->opt[i]))
		return ;
	if (unset(cmd, env, i) == 1)
	{
		env_line = fill_dishwasher(ft_strdup(cmd->opt[i]), \
		free, get_sink(NULL));
		new_line = fill_dishwasher(ft_lstnew(env_line), \
		free, get_sink(NULL));
		if (!new_line || !new_line)
		{
			perror("malloc error");
			return ;
		}
		ft_lstadd_back(env, new_line);
	}
}

void	export(t_exec *cmd, t_list **env, int is_parent)
{
	int		i;

	i = 1;
	if (!cmd->opt[1] && !is_parent)
	{
		print_exp_env(env);
		clean_exit(cmd->bin, EXIT_SUCCESS);
	}
	while (cmd->opt[i])
	{
		if (cmd->opt[i] && is_parent)
			export_vars(cmd, env, i);
		i++;
	}
	if (!is_parent)
		clean_exit(cmd->bin, EXIT_SUCCESS);
}

int	calc_var_size(t_exec *cmd, int i, int *have_equal)
{
	int		var_size;

	var_size = 0;
	while (cmd->opt[i][var_size] && cmd->opt[i][var_size] != '=')
		var_size++;
	if (cmd->opt[i][var_size])
		var_size++;
	else
		have_equal = 0;
	return (var_size);
}

char	*find_data_ref(t_list **env, t_exec *cmd, int i, int var_size)
{
	char	*data_ref;
	t_list	*ptr;

	ptr = *env;
	data_ref = NULL;
	while (ptr)
	{
		if (!ft_strncmp(ptr->content, cmd->opt[i], var_size))
		{
			data_ref = ptr->content;
			break ;
		}
		ptr = ptr->next;
	}
	return (data_ref);
}

int	unset_var(char *data_ref, int have_equal, int index, t_list **env)
{
	if (data_ref && (have_equal || !index))
	{
		ft_lst_remove_if(env, data_ref, ft_strcmp, 0);
		if (index)
			return (1);
	}
	else if (data_ref)
		return (2);
	return (0);
}

int	unset(t_exec *cmd, t_list **env, int index)
{
	char	*data_ref;
	int		var_size;
	int		i;
	int		have_equal;
	int		unset_val;

	i = 1;
	unset_val = 0;
	have_equal = 1;
	if (!cmd->opt[1])
		return (0);
	if (index)
		i = index;
	while (cmd->opt[i])
	{
		var_size = calc_var_size(cmd, i, &have_equal);
		data_ref = find_data_ref(env, cmd, i, var_size);
		unset_val = unset_var(data_ref, have_equal, index, env);
		if (unset_val)
			return (unset_val);
		if (index)
			break ;
		i++;
	}
	return (1);
}
