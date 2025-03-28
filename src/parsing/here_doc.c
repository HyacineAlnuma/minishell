/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:28:35 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/28 14:37:08 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_file_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] && EXEC_TMP_FILE[i])
	{
		if (str[i] != EXEC_TMP_FILE[i])
			return (str);
		i++;
	}
	return (&str[i + 1]);
}

int	format_here_doc(char *str, t_list **env, char **envp, t_sink **bin)
{
	int		i;
	size_t	j;
	char	*cmd;
	char	*formatted;
	int		buf_size;
	t_exec	**cmds;
	int		k = -1;
	int		temp_file_fd;
	int		hd_fd;
	char	*buffer;
	char	*char_buf;
	int		begin_part;
	int		g = 0;
	int		f_len;

	i = 0;
	j = 0;
	begin_part = 0;
	buffer = NULL;
	formatted = NULL;
	char_buf = ft_calloc(sizeof(char), 2);
	while (str && str[i])
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			j = i;
			while (str[++j])
			{
				if (str[j] == ')')
					break ;
			}
			if (j == ft_strlen(str))
			{
				perror("parse error");
				return (0);
			}
			str[i] = '\0';
			formatted = ft_strappend(formatted, &str[begin_part]);
			cmd = ft_strndup(&str[i + 2], (j - (i + 2)));
			i = j + 1;
			begin_part = i;
			cmds = parsing(cmd, env, bin);
			free(cmd);
			k = -1;
			while (cmds[++k])
				cmds[k]->here_doc = 1;
			exec(cmds, env, envp);
			temp_file_fd = open(EXEC_TMP_FILE, O_RDONLY);
			if (!temp_file_fd || temp_file_fd == -1)
			{
				perror("file error");
				return (0);
			}
			buf_size = ft_strlen(buffer);
			ft_bzero(buffer, buf_size);
			while (read(temp_file_fd, char_buf, 1))
				buffer = ft_strappend(buffer, char_buf);
			buffer = remove_file_name(buffer);
			g = 0;
			while (buffer[g])
			{
				if (buffer[g] == '\n' && !buffer[g + 1])
					buffer[g] = '\0';
				g++;
			}
			close(temp_file_fd);
			unlink(EXEC_TMP_FILE);
			formatted = ft_strappend(formatted, buffer);
		}
		i++;
	}
	formatted = ft_strappend(formatted, &str[begin_part]);
	// printf("%s\n", formatted);
	hd_fd = open(HD_TEMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	f_len = ft_strlen(formatted);
	write(hd_fd, formatted, f_len);
	close(hd_fd);
	hd_fd = open(HD_TEMP_FILE, O_RDONLY);
	if (*buffer)
		free(buffer);
	free(char_buf);
	free(formatted);
	free(envp);
	return (hd_fd);
}

// int	main(int ac, char **av, char **envp)
//  {
//  	char *s = "yo $(ls)    ";
//  	int	hd_fd;
// 	char	formatted[500];
//  	char *dup;
//  	t_list	**env;
// 	int		i;
 
//  	env = lst_env(envp);
//  	dup = ft_strdup(s);
//  	hd_fd = format_here_doc(dup, env, envp);
// 	// hd_fd = open(HD_TEMP_FILE, O_RDONLY);
// 	i = read(hd_fd, formatted, 500);
// 	formatted[i] = '\0';
// 	printf("%s\n", formatted);
//  	// printf("%s\n", s);
//  	(void)ac;
//  	(void)av;
//  }
