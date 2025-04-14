/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:32:09 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/14 14:14:55 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_cmd_exists(char **paths, char *cmd)
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
		path = fill_dishwasher(ft_strjoin(path, cmd), free, get_sink(NULL));
		if (!path)
		{
			perror("malloc error");
			return (2);
		}
		if (!access(path, F_OK))
			return (1);
		path = &paths[i][j + 1];
		i++;
	}
	return (0);
}

int	check_cmd_no_env(char *cmd)
{
	char	*paths[12];

	paths[0] = "/home/halnuma/bin";
	paths[1] = "/usr/local/sbin";
	paths[2] = "/usr/local/bin";
	paths[3] = "/usr/sbin";
	paths[4] = "/bin";
	paths[5] = "/usr/games";
	paths[6] = "/usr/local/games";
	paths[7] = "/snap/bin";
	paths[8] = "/home/halnuma/.dotnet/tools";
	paths[9] = "/usr/bin";
	paths[10] = "/sbin";
	paths[11] = NULL;
	return (check_if_cmd_exists(paths, cmd));
}

int	check_cmd_with_env(char *cmd, char *paths)
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
			path = fill_dishwasher(ft_strjoin(path, cmd), free, get_sink(NULL));
			if (!access(path, F_OK))
				return (1);
			path = &paths[i + 1];
		}
	}
	return (free(paths), 0);
}

int	check_cmd(char *cmd)
{
	char		*paths;
	struct stat	fs;

	if (check_builtins(cmd))
		return (1);
	if (stat(cmd, &fs) == 0 && !S_ISREG(fs.st_mode))
		return (3);
	if (stat(cmd, &fs) == 0 && (fs.st_mode & S_IXUSR))
		return (1);
	paths = ft_strdup(getenv("PATH"));
	if (!paths)
		return (check_cmd_no_env(cmd));
	else
		return (check_cmd_with_env(cmd, paths));
}
