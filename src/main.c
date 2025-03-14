/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:41:15 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/14 15:09:18 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_exec	*init_struct(void)
// {
// 	t_exec	*cmd;

// 	cmd = malloc(sizeof(t_exec));
// 	cmd->opt = malloc(sizeof(char *) * 3);
// 	cmd->cmd = "ls";
// 	cmd->opt[0] = "";
// 	cmd->infile = NULL;
// 	cmd->outfile = NULL;
// 	cmd->append = 0;
// 	//cmd->opt[1] = "test.txt";
// 	// cmd->opt[2] = "test";
// 	return (cmd);
// }

// t_exec	*init_struct2(void)
// {
// 	t_exec	*cmd;

// 	cmd = malloc(sizeof(t_exec));
// 	cmd->opt = malloc(sizeof(char *) * 2);
// 	cmd->cmd = "cat";
// 	cmd->opt[0] = "";
// 	//cmd->opt[1] = "CODE";
// 	cmd->infile = "y.txt";
// 	cmd->outfile = "test.txt";
// 	cmd->append = 0;
// 	// cmd->opt[2] = "test";
// 	return (cmd);
// }

// t_exec	*init_struct3(void)
// {
// 	t_exec	*cmd;

// 	cmd = malloc(sizeof(t_exec));
// 	cmd->opt = malloc(sizeof(char *) * 2);
// 	cmd->cmd = "wc";
// 	cmd->opt[0] = "";
// 	cmd->opt[1] = "-l";
// 	cmd->infile = NULL;
// 	cmd->outfile = NULL;
// 	cmd->append = 0;
// 	// cmd->opt[2] = "test";
// 	return (cmd);
// }

// t_exec	*init_struct4(void)
// {
// 	t_exec	*cmd;

// 	cmd = malloc(sizeof(t_exec));
// 	cmd->opt = malloc(sizeof(char *) * 2);
// 	cmd->cmd = "grep";
// 	cmd->opt[0] = "";
// 	cmd->opt[1] = "5";
// 	cmd->infile = NULL;
// 	cmd->outfile = NULL;
// 	cmd->append = 0;
// 	// cmd->opt[2] = "test";
// 	return (cmd);
// }

// t_exec	*init_struct5(void)
// {
// 	t_exec	*cmd;

// 	cmd = malloc(sizeof(t_exec));
// 	cmd->opt = malloc(sizeof(char *) * 2);
// 	cmd->cmd = "wc";
// 	cmd->opt[0] = "";
// 	cmd->opt[1] = "-l";
// 	cmd->infile = NULL;
// 	cmd->outfile = NULL;
// 	cmd->append = 0;
// 	// cmd->opt[2] = "test";
// 	return (cmd);
// }

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
	char	*str;
	int		i;
	t_list	*ptr;

	ptr = *env;
	while (ptr)
	{
		if (!ft_strncmp(ptr->content, "OLDPWD=", 4))
		{
			i = -1;
			str = ptr->content;
			while (str[++i])
			{
				if (str[i] == '=')
				{
					i++;
					break ;
				}
			}
			previouspwd = &str[i];
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

	if (!cmd->opt[1])
		return ;
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


int	main(int ac, char **av, char **envp)
{
	t_list	*new_line;

	unset(cmd, env);
	new_line = ft_lstnew(cmd->opt[1]);
	if (!new_line)
		return ;
	ft_lstadd_back(env, new_line);
	//printf("%s\n", (char *)ft_lstlast(*env)->content);
}

void	free_all(t_list **env, t_exec **cmds, int *pid)
{
	int		i;
	int		j;

	ft_lstclear(env, NULL);
	i = 0;
	free(pid);
	i = 0;
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

void	exit_program(t_exec **cmds, t_list **env, int *pid)
{
	// pid_t	ppid;

	// ppid = getppid();
	// kill(ppid, SIGUSR1);
	free_all(env, cmds, pid);
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
		echo(cmd);
	else if (!ft_strncmp(cmd->cmd, "cd", 3))
		exit(EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "pwd", 4))
		pwd();
	else if (!ft_strncmp(cmd->cmd, "export", 7))
		exit(EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "unset", 6))
		exit(EXIT_SUCCESS);
	else if (!ft_strncmp(cmd->cmd, "env", 4))
		print_env(env);
	else if (!ft_strncmp(cmd->cmd, "exit", 5))
		exit(EXIT_SUCCESS);
	return (0);
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
	if (!access(cmd, F_OK))
		return (1);
	i = 0;
	paths = getenv("PATH");
	paths = ft_strjoin(paths, ":");
	path = paths;
	while (paths[i])
	{
		if (paths[i] == ':')
		{
			paths[i] = '\0';
			path = ft_strjoin(path, "/");
			path = ft_strjoin(path, cmd);
			if (!access(path, F_OK))
				return (1);
			path = &paths[i + 1];
		}
		i++;
	}
	return (0);
}

void	manage_files(t_exec *cmd)
{
	int		outfile_fd[1024];
	int		infile_fd[1024];
	// int		str_len;
	int		i;

	if (cmd->outfile)
	{
		i = -1;
		while (cmd->outfile[++i])
		{
			if (cmd->append[i])
				outfile_fd[i] = open(cmd->outfile[i], O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | O_APPEND);
			else
				outfile_fd[i] = open(cmd->outfile[i], O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
			dup2(outfile_fd[i], STDOUT_FILENO);
			close(outfile_fd[i]);
		}
	}
	if (cmd->infile)
	{
		i = -1;
		while (cmd->outfile[++i])
		{
			infile_fd[i] = open(cmd->infile[i], O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
			dup2(infile_fd[i], STDIN_FILENO);
			close(infile_fd[i]);
		}
	}
	if (cmd->here_doc == 1)
	{
			infile_fd[i] = open(cmd->infile[i], O_RDWR | S_IWUSR | S_IRUSR);
			dup2(infile_fd[i], STDIN_FILENO);
			close(infile_fd[i]);
	}
}

void	close_pipes(int *pipefd, int pipe_nb)
{
	int	i;

	i = 0;
	while (i < pipe_nb * 2)
	{
		close(pipefd[i]);
		i++;
	}
}

void	open_pipes(int *pipefd, int pipe_nb)
{
	int	i;

	i = 0;
	while (i < pipe_nb)
	{
		if (pipe(pipefd + i * 2) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	dup_pipes(t_exec **cmds, int *pipefd, int cur_cmd, int cur_pipe)
{
	int	temp_file_fd;
	//char buffer[1000];

	if (cmds[cur_cmd + 1])
		dup2(pipefd[cur_pipe + 1], STDOUT_FILENO);
	if (cur_cmd)
		dup2(pipefd[cur_pipe - 2], STDIN_FILENO);
	if (!cmds[cur_cmd + 1] && cmds[cur_cmd]->here_doc == 2)
	{
		temp_file_fd = open(HD_TEMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
		dup2(temp_file_fd, STDOUT_FILENO);
		close(temp_file_fd);
	}
	if (!cmds[cur_cmd + 1] && cmds[cur_cmd]->here_doc > 2)
	{
		dup2(cmds[cur_cmd]->here_doc, STDOUT_FILENO);
		close(cmds[cur_cmd]->here_doc);
	}
}

void	exec_cmd(t_exec *cmd, t_list **env, char **envp)
{
	char	*exe;
	char	*path;
	char	pwd[PATH_MAX];

	if (!check_cmd(cmd->cmd))
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found.\n", 2);
		exit(CMD_NOT_FOUND);
	}
	else if (!exec_builtins(cmd, env))
	{
		if (!ft_strcmp(cmd->cmd, "./"))
		{
			getcwd(pwd, sizeof(pwd));
			cmd->cmd[0] = '/';
			path = pwd;
		}
		else
			path = "/bin/";
		exe = ft_strjoin(path, cmd->cmd);
		execve(exe, cmd->opt, envp);
	}
}

void	exec_parent_builtins(t_exec **cmds, t_exec *cmd, t_list **env, pid_t *pid)
{
	if (!ft_strncmp(cmd->cmd, "cd", 3))
		cd(cmd, env);
	if (!ft_strncmp(cmd->cmd, "exit", 5))
		exit_program(cmds, env, pid);
	if (!ft_strncmp(cmd->cmd, "export", 7))
		export(cmd, env);
	if (!ft_strncmp(cmd->cmd, "unset", 6))
		unset(cmd, env);
}

int	last_status_code(int status, int instruction)
{
	static int	status_code;

	if (!instruction && WIFEXITED(status))
		status_code = WEXITSTATUS(status);
	else
		return (status_code);
	return (0);
}

void	wait_all_pid(pid_t *pid, int pipe_nb)
{
	int		i;
	int		status;
	// int		t;

	i = 0;
	status = 0;
	while (i < (pipe_nb + 1))
	{
		waitpid(pid[i], &status, 0);
		last_status_code(status, 0);
		i++;
	}
	// t = last_status_code(0, 1);
	// printf("status:%d\n", t);
}

void	exec_process(t_fork *f, t_list **env, char **envp)
{
	f->pid[f->cur_cmd] = fork();
	if (f->pid[f->cur_cmd] == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (f->pid[f->cur_cmd] == 0)
	{
		//manage_files(f->cmds[f->cur_cmd]);		\\ A rajouter quand le parsing gerera les files
		dup_pipes(f->cmds, f->pipefd, f->cur_cmd, f->cur_pipe);
		close_pipes(f->pipefd, f->pipe_nb);
		exec_cmd(f->cmd, env, envp);
	}
}

void	init_fork(t_fork *fork_info, t_exec **cmds, int pipe_nb, int cur_pipe)
{
	fork_info->cmds = cmds;
	fork_info->pipe_nb = pipe_nb;
	fork_info->cur_pipe = cur_pipe;
}

void	init_fork_bis(t_fork *f, int *pfd, t_exec *cmd, int cur_cmd, int *pid)
{
	f->cmd = cmd;
	f->pipefd = pfd;
	f->cur_cmd = cur_cmd;
	f->pid = pid;
}

int	exec_cmds(t_exec **cmds, char **envp, t_list **env, int pipe_nb, pid_t *pid)
{
	int		cur_cmd;
	int		cur_pipe;
	int		pipefd[1024];
	t_fork	*fork_info;

	cur_cmd = 0;
	cur_pipe = 0;
	open_pipes(pipefd, pipe_nb);
	while (cmds[cur_cmd])
	{
		fork_info = ft_calloc(sizeof(t_fork), 1);
		if (!fork_info)
			return (0);
		init_fork(fork_info, cmds, pipe_nb, cur_pipe);
		init_fork_bis(fork_info, pipefd, cmds[cur_cmd], cur_cmd, pid);
		exec_process(fork_info, env, envp);
		cur_cmd++;
		cur_pipe += 2;
	}
	exec_parent_builtins(cmds, cmds[--cur_cmd], env, pid);
	close_pipes(pipefd, pipe_nb);
	wait_all_pid(pid, pipe_nb);
	unlink(HD_TEMP_FILE);
	//free_all(env, cmds, pid);
	return (1);
}


void	exec(t_exec **cmds, t_list **env, char **envp)
{
	int		pipe_nb;
	pid_t	*pid;

	pipe_nb = ft_tablen((char **)cmds) - 1;
	pid = malloc(sizeof(int) * (pipe_nb + 1));
	// printf("pipenb:%d\n", pipe_nb);
	// (void)envp;
	// (void)env;
	exec_cmds(cmds, envp, env, pipe_nb, pid);
}

// int	main(int ac, char **av, char **envp)
// {
// 	t_exec	*tcmds[3];

// 	(void)ac;
// 	(void)av;
// 	tcmds[0] = init_struct();
// 	tcmds[1] = init_struct2();
// 	// tcmds[2] = init_struct3();
// 	// tcmds[3] = init_struct4();
// 	// tcmds[4] = init_struct5();
// 	tcmds[2] = NULL;
// 	exec_cmds(tcmds, envp);
// }
