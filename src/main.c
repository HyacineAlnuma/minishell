/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:41:15 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/07 13:57:51 by halnuma          ###   ########.fr       */
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
	if (!ft_strncmp(cmd->opt[1], "-n", 2))
		ft_printf("%s", cmd->opt[2]);
	else
		ft_printf("%s\n", cmd->opt[1]);
	exit(EXIT_SUCCESS);
}

void	cd(t_exec *cmd)
{
	chdir(cmd->opt[1]);
	exit(EXIT_SUCCESS);
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

void	export(t_exec *cmd, t_list **env)
{
	t_list	*new_line;

	new_line = ft_lstnew(cmd->opt[1]);
	if (!new_line)
		return ;
	ft_lstadd_back(env, new_line);
	exit(EXIT_SUCCESS);
}

void	unset(t_exec *cmd, t_list **env)
{
	char	*data_ref;
	t_list	*ptr;
	int		var_size;

	var_size = ft_strlen(cmd->opt[1]);
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
	ft_lst_remove_if(env, data_ref, ft_strncmp);
	exit(EXIT_SUCCESS);
}

void	exit_program(t_exec *cmd)
{
	free(cmd->opt);
	free(cmd);
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

t_list	**lst_enve(char **envp)
{
	int		i;
	t_list	**lst_env;
	t_list	*new_line;

	i = 0;
	lst_env = (t_list **)malloc(sizeof(t_list **));
	if (!lst_env)
		return (NULL);
	while (envp[i])
	{
		new_line = ft_lstnew(envp[i]);
		if (!new_line)
			return (NULL);
		ft_lstadd_back(lst_env, new_line);
		i++;
	}
	return (lst_env);
}


void	exec_builtins(t_exec *cmd, t_list **env)
{
	if (!ft_strncmp(cmd->cmd, "echo", 5))
		echo(cmd);
	else if (!ft_strncmp(cmd->cmd, "cd", 3))
		cd(cmd);
	else if (!ft_strncmp(cmd->cmd, "pwd", 4))
		pwd();
	else if (!ft_strncmp(cmd->cmd, "export", 7))
		export(cmd, env);
	else if (!ft_strncmp(cmd->cmd, "unset", 6))
		unset(cmd, env);
	else if (!ft_strncmp(cmd->cmd, "env", 4))
		print_env(env);
	else if (!ft_strncmp(cmd->cmd, "exit", 5))
		exit_program(cmd);
}

void	exec_cmds(t_exec **cmds, char **envp, t_list **env)
{
	int		i = 0;
	int		j = 0;
	int		k = 0;
	pid_t	*pid;
	int		cmd_nb;
	char	*path;
	char	*exe;

	cmd_nb = ft_tablen((char **)cmds);
	pid = malloc(sizeof(int) * cmd_nb);
	int		pipefd[2 * cmd_nb];
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
			exec_builtins(cmds[j], env);
			path = "/bin/";
			exe = ft_strjoin(path, cmds[j]->cmd);
			execve(exe, cmds[j]->opt, envp);
		}
		j++;
		k += 2;
	}
	i = 0;
	while (i < cmd_nb * 2)
	{
		close(pipefd[i]);
		i++;
	}
	i = 0;
	while (i < cmd_nb)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

void	exec(t_exec **cmds, t_list **env, char **envp)
{
	//t_exec	*cmds[6];
	//int i = 0;

	// cmds[0] = init_struct();
	// cmds[1] = init_struct2();
	// cmds[2] = init_struct3();
	// cmds[3] = init_struct4();
	// cmds[4] = init_struct5();
	// cmds[5] = NULL;
	exec_cmds(cmds, envp, env);
}
