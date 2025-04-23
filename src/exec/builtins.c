/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:32:53 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/23 09:45:37 by secros           ###   ########.fr       */
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
	else
		return (2);
	return (1);
}

int	add_old_pwd(t_list **env, char *oldpwd)
{
	t_list	*new;

	new = fill_dishwasher(
			ft_lstnew(fill_dishwasher(
					ft_strjoin("OLDPWD=", oldpwd), free, get_sink(NULL)
					)), free, get_sink(NULL)
			);
	if (!new || !new->content)
	{
		perror("Malloc error");
		return (0);
	}
	ft_lstadd_back(env, new);
	return (1);
}

int	change_dir(t_exec *cmd, t_list *ptr, char *oldpwd, t_list **env)
{
	char	pwd[PATH_MAX];
	int		pwd_val;

	if (!chdir(cmd->opt[1]))
	{
		getcwd(pwd, sizeof(pwd));
			// perror("getcwd() error");
		while (ptr)
		{
			pwd_val = change_pwd_vars(ptr, oldpwd, pwd);
			if (!pwd_val)
				return (0);
			ptr = ptr->next;
		}
		if (pwd_val == 2)
			if (!add_old_pwd(env, oldpwd))
				return (0);
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
		cmd->opt[1] = find_node(env, "$HOME");
		if (!cmd->opt[1])
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return ((void)last_status_code(1, 2));
		}
	}
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		perror("getcwd() error");
	ptr = *env;
	previouspwd = get_previous_pwd(env);
	if (!ft_strncmp(cmd->opt[1], "-", 2))
	{
		if (!previouspwd)
			return ((void)printf("minishell: cd: OLDPWD not set\n"));
		cmd->opt[1] = previouspwd;
	}
	if (!change_dir(cmd, ptr, oldpwd, env))
		return ;
}
