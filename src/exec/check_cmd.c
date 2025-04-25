/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:32:09 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/25 09:21:55 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_cmd_exists(char **paths, t_exec *cmd)
{
	char	*path;
	int		i;

	path = NULL;
	i = -1;
	while (paths[++i])
	{
		path = paths[i];
		path = fill_dishwasher(ft_strjoin(path, "/"), free, get_sink(NULL));
		path = fill_dishwasher(ft_strjoin(path, cmd->cmd), \
		free, get_sink(NULL));
		if (!path)
		{
			perror("malloc error");
			return (2);
		}
		if (!access(path, F_OK))
		{
			cmd->path = path;
			return (1);
		}
	}
	return (0);
}

int	check_cmd_no_env(t_exec *cmd)
{
	char	*paths[10];

	paths[0] = "/usr/local/sbin";
	paths[1] = "/usr/local/bin";
	paths[2] = "/usr/sbin";
	paths[3] = "/bin";
	paths[4] = "/usr/games";
	paths[5] = "/usr/local/games";
	paths[6] = "/snap/bin";
	paths[7] = "/usr/bin";
	paths[8] = "/sbin";
	paths[9] = NULL;
	return (check_if_cmd_exists(paths, cmd));
}

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

	paths = fill_dishwasher(ft_strjoin(paths, ":"), free, get_sink(NULL));
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
		return (check_cmd_no_env(cmd));
	else
	{
		paths = ft_strdup(paths);
		if (!paths)
			return (4);
		return (check_cmd_with_env(cmd, paths));
	}
}
