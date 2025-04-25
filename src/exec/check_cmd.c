/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:32:09 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/25 11:20:13 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_cmd_path(char *paths, char **path, int i, t_exec *cmd)
{
	paths[i] = '\0';
	*path = fill_dishwasher(ft_strjoin(*path, "/"), free, get_sink(NULL));
	if (!path)
	{
		perror("malloc error");
		free(paths);
		return (0);
	}
	*path = fill_dishwasher(ft_strjoin(*path, cmd->cmd), \
	free, get_sink(NULL));
	if (!*path)
	{
		perror("malloc error");
		free(paths);
		return (0);
	}
	if (!access(*path, F_OK | X_OK))
	{
		cmd->path = *path;
		return (1);
	}
	*path = &paths[i + 1];
	return (2);
}

int	check_cmd_with_env(t_exec *cmd, char *paths)
{
	char	*path;
	int		i;
	int		check_cmd_val;

	paths = fill_dishwasher(ft_strappend(paths, ":"), free, get_sink(NULL));
	if (!paths)
	{
		perror("malloc error");
		return (2);
	}
	path = paths;
	i = -1;
	if (strchr(cmd->cmd, '/'))
		return (1);
	while (paths[++i])
	{
		if (paths[i] == ':')
		{
			check_cmd_val = check_cmd_path(paths, &path, i, cmd);
			if (!check_cmd_val || check_cmd_val == 1)
				return (check_cmd_val);
		}
	}
	return (0);
}

int	check_cmd(t_exec *cmd, t_list **env)
{
	char		*paths;
	struct stat	fs;

	if (check_builtins(cmd->cmd))
		return (1);
	if (stat(cmd->cmd, &fs) == 0 && !S_ISREG(fs.st_mode))
		return (3);
	paths = find_node(env, "$PATH");
	if (!paths)
	{
		print_error(NULL, NULL, "PATH is not set");
		return (5);
	}
	else
		paths = ft_strdup(paths);
	if (!paths)
		return (4);
	return (check_cmd_with_env(cmd, paths));
}
