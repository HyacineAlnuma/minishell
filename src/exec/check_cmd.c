/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:32:09 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/18 15:43:20 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_cmd_exists(char **paths, t_exec *cmd)
{
	char	*path;
	int		i;
	int		j;

	path = NULL;
	i = 0;
	j = 0;
	while (paths[i])
	{
		path = paths[i];
		path = fill_dishwasher(ft_strjoin(path, "/"), free, get_sink(NULL));
		path = fill_dishwasher(ft_strjoin(path, cmd->cmd), free, get_sink(NULL));
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
		path = &paths[i][j + 1];
		i++;
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

int	check_cmd_with_env(t_exec *cmd, char *paths)
{
	char	*path;
	int		i;

	path = NULL;
	path = fill_dishwasher(ft_strjoin(path, ":"), free, get_sink(NULL));
	if (!path)
	{
		perror("malloc error");
		return (2);
	}
	path = paths;
	i = -1;
	while (paths[++i])
	{
		if (paths[i] == ':')
		{
			paths[i] = '\0';
			path = fill_dishwasher(ft_strjoin(path, "/"), free, get_sink(NULL));
			if (!path)
			{
				perror("malloc error");
				return (free(paths), 0);
			}
			path = fill_dishwasher(ft_strjoin(path, cmd->cmd), free, get_sink(NULL));
			if (!path)
			{
				perror("malloc error");
				return (free(paths), 0);
			}
			if (!access(path, F_OK))
			{
				cmd->path = path;
				return (1);
			}
			path = &paths[i + 1];
		}
	}
	return (free(paths), 0);
}

int	check_cmd(t_exec *cmd)
{
	char		*paths;
	struct stat	fs;

	if (check_builtins(cmd->cmd))
		return (1);
	if (stat(cmd->cmd, &fs) == 0 && !S_ISREG(fs.st_mode))
		return (3);
	if (stat(cmd->cmd, &fs) == 0 && (fs.st_mode & S_IXUSR))
		return (1);
	paths = getenv("PATH");
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
