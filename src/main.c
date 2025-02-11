/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:41:15 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/11 12:56:13 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec	*init_struct(void)
{
	t_exec	*cmd;

	cmd = malloc(sizeof(t_exec));
	cmd->opt = malloc(sizeof(char *) * 3);
	cmd->cmd = "env";
	cmd->opt[0] = "";
	//cmd->opt[1] = "test.txt";
	// cmd->opt[2] = "test";
	return (cmd);
}

t_exec	*init_struct2(void)
{
	t_exec	*cmd;

	cmd = malloc(sizeof(t_exec));
	cmd->opt = malloc(sizeof(char *) * 2);
	cmd->cmd = "grep";
	cmd->opt[0] = "";
	cmd->opt[1] = "CODE";
	// cmd->opt[2] = "test";
	return (cmd);
}

t_exec	*init_struct3(void)
{
	t_exec	*cmd;

	cmd = malloc(sizeof(t_exec));
	cmd->opt = malloc(sizeof(char *) * 2);
	cmd->cmd = "wc";
	cmd->opt[0] = "";
	cmd->opt[1] = "-l";
	// cmd->opt[2] = "test";
	return (cmd);
}

t_exec	*init_struct4(void)
{
	t_exec	*cmd;

	cmd = malloc(sizeof(t_exec));
	cmd->opt = malloc(sizeof(char *) * 2);
	cmd->cmd = "grep";
	cmd->opt[0] = "";
	cmd->opt[1] = "5";
	// cmd->opt[2] = "test";
	return (cmd);
}

t_exec	*init_struct5(void)
{
	t_exec	*cmd;

	cmd = malloc(sizeof(t_exec));
	cmd->opt = malloc(sizeof(char *) * 2);
	cmd->cmd = "wc";
	cmd->opt[0] = "";
	cmd->opt[1] = "-l";
	// cmd->opt[2] = "test";
	return (cmd);
}

void	echo(t_exec *cmd)
{
	int	i;

	i = 1;
	if (!ft_strncmp(cmd->opt[1], "-n", 3))
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
	exit(EXIT_SUCCESS);
}

char	*get_previous_pwd(t_list **env)
{
	char	*previouspwd;
	char	*yo;
	int		i;
	t_list	*ptr;

	ptr = *env;
	while (ptr)
	{
		if (!ft_strncmp(ptr->content, "OLDPWD=", 4))
		{
			i = 0;
			yo = ptr->content;
			while (yo[i])
			{
				if (yo[i] == '=')
				{
					i++;
					break ;
				}
				i++;
			}
			previouspwd = &yo[i];
		}
		ptr = ptr->next;
	}
	return (previouspwd);
}

void	cd(t_exec *cmd, t_list **env)
{
	t_list	*ptr;
	char	pwd[PATH_MAX];
	char	oldpwd[PATH_MAX];
	char	*previouspwd;

	if (!getcwd(oldpwd, sizeof(oldpwd)))
		perror("getcwd() error");
	ptr = *env;
	previouspwd = get_previous_pwd(env);
	if (!ft_strncmp(cmd->opt[1], "-", 2))
		cmd->opt[1] = previouspwd;
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

void	pwd(void)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_printf("%s\n", pwd);
	else
		perror("getcwd() error");
	exit(EXIT_SUCCESS);
}

void	unset(t_exec *cmd, t_list **env)
{
	char	*data_ref;
	t_list	*ptr;
	int		var_size;

	var_size = 0;
	while(cmd->opt[1][var_size] && cmd->opt[1][var_size] != '=')
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

void	export(t_exec *cmd, t_list **env)
{
	t_list	*new_line;

	unset(cmd, env);
	new_line = ft_lstnew(cmd->opt[1]);
	if (!new_line)
		return ;
	ft_lstadd_back(env, new_line);
}

void	free_all(t_list **env, t_exec **cmds)
{
	int		i;
	int		j;

	ft_lstclear(env, NULL);
	i = 0;
	(void)env;
	// (void)j;
	while (cmds[i])
	{
		//free(cmds[i]->cmd);
		j = 0;
		while (cmds[i]->opt[j])
		{
			free(cmds[i]->opt[j]);
			j++;
		}
		free(cmds[i]->opt);
		i++;
	}
	free(cmds);
}

void	exit_program(t_exec **cmds, t_list **env)
{
	// pid_t	ppid;

	// ppid = getppid();
	// kill(ppid, SIGUSR1);
	free_all(env, cmds);
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


int	exec_builtins(t_exec *cmd, t_list **env)
{
	if (!ft_strncmp(cmd->cmd, "echo", 5))
	{
		echo(cmd);
		return (1);
	}
	else if (!ft_strncmp(cmd->cmd, "cd", 3))
	{
		exit(EXIT_SUCCESS);
		return (1);
	}
	else if (!ft_strncmp(cmd->cmd, "pwd", 4))
	{
		pwd();
		return (1);
	}
	else if (!ft_strncmp(cmd->cmd, "export", 7))
	{
		exit(EXIT_SUCCESS);
		return (1);
	}
	else if (!ft_strncmp(cmd->cmd, "unset", 6))
	{
		exit(EXIT_SUCCESS);
		return (1);
	}
	else if (!ft_strncmp(cmd->cmd, "env", 4))
	{
		print_env(env);
		return (1);
	}
	else if (!ft_strncmp(cmd->cmd, "exit", 5))
	{
		exit(EXIT_SUCCESS);
		return (1);
	}
	return (0);
}

void	sig_handler(int signum)
{
	if (signum == SIGUSR1)
		exit(EXIT_SUCCESS);
}

int	check_builtins(char *cmd)
{
	if (!ft_strncmp(cmd, "echo", 5))
		return (1);
	else if (!ft_strncmp(cmd, "cd", 3))
		return (1);
	else if (!ft_strncmp(cmd, "pwd", 4))
		return (1);
	else if (!ft_strncmp(cmd, "export", 7))
		return (1);
	else if (!ft_strncmp(cmd, "unset", 6))
		return (1);
	else if (!ft_strncmp(cmd, "env", 4))
		return (1);
	else if (!ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
}

int	check_cmd(char *cmd)
{
	char	*paths;
	char	*path;
	int		i;

	if (check_builtins(cmd))
		return (1);
	i = 0;
	paths = getenv("PATH");
	paths = ft_strjoin(paths, ":");
	//printf("%s\n", paths);
	path = paths;
	while (paths[i])
	{
		if (paths[i] == ':')
		{
			paths[i] = '\0';
			path = ft_strjoin(path, "/");
			path = ft_strjoin(path, cmd);
			//printf("%s\n", path);
			if (!access(path, F_OK))
				return (1);
			path = &paths[i + 1];
		}
		i++;
	}
	return (0);
}

void	exec_cmds(t_exec **cmds, char **envp)
{
	int		i = 0;
	int		j = 0;
	int		k = 0;
	pid_t	*pid;
	int		cmd_nb;
	char	*exe;
	char	*path;
	int	status = 0;
	t_list	**env;

	//signal(SIGUSR1, &sig_handler);
	cmd_nb = ft_tablen((char **)cmds);
	pid = malloc(sizeof(int) * cmd_nb);
	int		pipefd[2 * cmd_nb];
	env = lst_env(envp);
	while (i < cmd_nb)
	{
		if (pipe(pipefd + i * 2) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	while (cmds[j])
	{
		if ((pid[j] = fork()) == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid[j] == 0)
		{
			//next = 0;
			if (cmds[j + 1])
				dup2(pipefd[k + 1], 1);
			if (j)
				dup2(pipefd[k - 2], 0);
			i = 0;
			while (i < cmd_nb * 2)
			{
				close(pipefd[i]);
				i++;
			}
			if (!check_cmd(cmds[j]->cmd))
			{
				//printf("%s: command not found.\n", cmds[j]->cmd);
				ft_putstr_fd(cmds[j]->cmd, 2);
				ft_putstr_fd(": command not found.\n", 2);
				exit(EXIT_FAILURE);
			}
			else if (!exec_builtins(cmds[j], env))
			{
				path = "/bin/";
				exe = ft_strjoin(path, cmds[j]->cmd);
				execve(exe, cmds[j]->opt, envp);
			}
		}
		j++;
		k += 2;
	}
	j--;
	if (!ft_strncmp(cmds[j]->cmd, "cd", 3))
		cd(cmds[j], env);
	if (!ft_strncmp(cmds[j]->cmd, "exit", 5))
		exit_program(cmds, env);
	if (!ft_strncmp(cmds[j]->cmd, "export", 7))
		export(cmds[j], env);
	if (!ft_strncmp(cmds[j]->cmd, "unset", 6))
		unset(cmds[j], env);
	i = 0;
	while (i < cmd_nb * 2)
	{
		close(pipefd[i]);
		i++;
	}
	i = 0;
	while (i < cmd_nb)
	{
		waitpid(pid[i], &status, 0);
		//printf("process:%d status:%d\n", i, status);
		i++;
	}
	free_all(env, cmds);
}


void	exec(t_exec **cmds, char **envp)
{
	//t_exec	*cmds[6];
	//int i = 0;

	// cmds[0] = init_struct();
	// cmds[1] = init_struct2();
	// cmds[2] = init_struct3();
	// cmds[3] = init_struct4();
	// cmds[4] = init_struct5();
	// cmds[5] = NULL;
	exec_cmds(cmds, envp);
}
