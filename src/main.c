/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:41:15 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/05 16:16:07 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec	*init_struct(void)
{
	t_exec	*cmd;

	cmd = malloc(sizeof(t_exec));
	cmd->opt = malloc(sizeof(char *) * 3);
	cmd->cmd = "cat";
	cmd->opt[0] = "";
	cmd->opt[1] = "test.txt";
	// cmd->opt[2] = "test";
	return (cmd);
}

t_exec	*init_struct2(void)
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

t_exec	*init_struct3(void)
{
	t_exec	*cmd;

	cmd = malloc(sizeof(t_exec));
	cmd->opt = malloc(sizeof(char *) * 2);
	cmd->cmd = "pwd";
	cmd->opt[0] = "";
	//cmd->opt[1] = "FOO=bar";
	// cmd->opt[2] = "test";
	return (cmd);
}

void	echo(t_exec *cmd)
{
	if (!ft_strncmp(cmd->opt[1], "-n", 2))
		ft_printf("%s", cmd->opt[2]);
	else
		ft_printf("%s\n", cmd->opt[1]);
}

void	cd(t_exec *cmd)
{
	chdir(cmd->opt[1]);
}

void	pwd(void)
{
	char	pwd[PATH_MAX];

	// pwd = getenv("OLDPWD");
	// ft_printf("%s\n", pwd);
	//pwd = NULL;
	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_printf("%s\n", pwd);
	else
		perror("getcwd() error");
}

void	export(t_exec *cmd, t_list **env)
{
	t_list	*new_line;


	new_line = ft_lstnew(cmd->opt[1]);
	if (!new_line)
		return ;
	ft_lstadd_back(env, new_line);
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
}

t_list	**lst_env(char **envp)
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

int	main(int ac, char **av, char **envp)
{
	t_exec	*cmd;
	int		pipefd[2];
	char	*path;
	char	*exe;
	pid_t	pid1;
	pid_t	pid2;

	if (ac > 1)
	{
		(void)av;
	}
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		cmd = init_struct();
		path = "/bin/";
		exe = ft_strjoin(path, cmd->cmd);
		execve(exe, cmd->opt, envp);
	}
	else
	{
		//wait(NULL);
		pid2 = fork();
		if (pid2 == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid2 == 0)
		{
			close(pipefd[1]);
			dup2(pipefd[0], 0);
			cmd = init_struct2();
			path = "/bin/";
			exe = ft_strjoin(path, cmd->cmd);
			execve(exe, cmd->opt, envp);
		}
	}
}
// int	main(int ac, char **av, char **envp)
// {
// 	t_exec	*cmd;
// 	char	*path;
// 	char	*exe;
// 	t_list	**env;
// 	//pid_t	pid;
// 	pid_t	pid2;
// 	pid_t	pid3;
// 	pid_t	pid4;


// 	if (ac > 1)
// 	{
// 		(void)av;
// 	}
// 	env = lst_env(envp);
// 	// pid = fork();
// 	// if (pid == 0)
// 	// {
// 	// 	cmd = init_struct();
// 	// 	if (!ft_strncmp(cmd->cmd, "ls", 2))
// 	// 	{
// 	// 		path = "/bin/";
// 	// 		exe = ft_strjoin(path, cmd->cmd);
// 	// 		execve(exe, cmd->opt, envp);
// 	// 	}
// 	// }
// 	// else if (!ft_strncmp(cmd->cmd, "echo", 4))
// 	// 	echo(cmd);
// 	// else if (!ft_strncmp(cmd->cmd, "cd", 2))
// 	// 	cd(cmd);
// 	// else if (!ft_strncmp(cmd->cmd, "pwd", 3))
// 	// 	pwd();
// 	// else if (!ft_strncmp(cmd->cmd, "export", 6))
// 	// 	export(cmd, env);
// 	// else if (!ft_strncmp(cmd->cmd, "unset", 5))
// 	// 	unset(cmd, env);
// 	// else if (!ft_strncmp(cmd->cmd, "env", 3))
// 	// 	print_env(env);
// 	// else if (!ft_strncmp(cmd->cmd, "exit", 4))
// 	// 	exit_program(cmd);
// 	// cmd = init_struct3();
// 	// if (!ft_strncmp(cmd->cmd, "export", 6))
// 	// 	export(cmd, env);
// 	// cmd = init_struct();
// 	// if (!ft_strncmp(cmd->cmd, "env", 3))
// 	// 	print_env(env);
// 	//waitpid(pid, NULL, 0);
// 	pid4 = fork();
// 	if (pid4 == 0)
// 	{
// 		cmd = init_struct();
// 		if (!ft_strncmp(cmd->cmd, "pwd", 6))
// 			pwd();
// 		//exit(EXIT_SUCCESS);
// 	}
// 	//waitpid(pid4, NULL, 0);
// 	pid2 = fork();
// 	if (pid2 == 0)
// 	{
// 		cmd = init_struct2();
// 		if (!ft_strncmp(cmd->cmd, "cd", 2))
// 		{
// 			path = "/bin/";
// 			exe = ft_strjoin(path, cmd->cmd);
// 			execve(exe, cmd->opt, envp);
// 		}
// 	}
// 	//waitpid(pid2, NULL, 0);
// 	pid3 = fork();
// 	if (pid3 == 0)
// 	{
// 		cmd = init_struct();
// 		if (!ft_strncmp(cmd->cmd, "pwd", 6))
// 			pwd();
// 		//exit(EXIT_SUCCESS);
// 	}
// 	// if (pid3 == 0)
// 	// {
// 	// 	cmd = init_struct3();
// 	// 	if (!ft_strncmp(cmd->cmd, "pwd", 3))
// 	// 		pwd();
// 	// 	//exit(EXIT_SUCCESS);
// 	// }
// 	return (0);
// }
