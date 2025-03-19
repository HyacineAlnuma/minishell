/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/03/19 14:42:41 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/* 
	create the struct
		handle redirection (and heredoc) OK
			set token to infile / outfile OK
		if token = pipe OK
			new_struct and repeat with the other lst 
	when lst = null
		parsing end
		destroy the lst but keep content; lst_clear with NULL
		send strut to exec

*/

int	lst_len(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

void	print_lsts(t_list **lst)
{
	int		j = 0;

	while (lst[j])
	{
		ft_printf("[%s]", (char *)lst[j]->content);
		lst[j] = lst[j]->next;
		if (!lst[j])
		{
			ft_printf("\n");
			j++;
		}
	}
}

t_exec	**parsing(char *str, t_list **env, t_garb *bin)
{

	t_list	**piped;
	t_list	*tokens;
	t_list	*tmp;
	t_exec	**exec;
	int		count;
	int		i;
	int		j;
	char	**tab;

	(void)bin;
	if (!str)
		return (NULL);
	tokens = create_token_list(str, bin);
	if (!tokens)
		return (NULL);
	env_handling(tokens, env, bin);
	count = lst_count_char(tokens, '|');
	piped = cut_instruction(tokens, count);
	exec = ft_calloc(sizeof(t_exec *), count + 2);
	add_garbage(exec, free_the_mallocs, bin);
	while (i <= count)
	{
		exec[i] = ft_calloc(sizeof(t_exec), 1);
		exec[i]->docs = create_docs(piped[i]);
		add_garbage(exec[i]->docs, free_the_mallocs, bin);
		merge_all(piped[i]);
		ft_lst_remove_if(&piped[i], NULL, compare);
		tab = ft_calloc(sizeof(char *), lst_len(piped[i]) + 1);
		tmp = piped[i];
		j = 0;
		while (tmp)
		{
			tab[j++] = tmp->content;
			tmp = tmp->next;
		}
		exec[i]->cmd = tab[0];
		exec[i]->opt = tab;
		i++;
	}
	return (exec);
}

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

int	format_here_doc(char *str, t_list **env, char **envp)
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
	buffer = "\0";
	formatted = NULL;
	char_buf = ft_calloc(sizeof(char), 2);
	while (str[i])
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
			cmds = parsing(cmd, env, NULL);
			k = -1;
			while (cmds[++k])
				cmds[k]->here_doc = 1;
			// list to tab
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
				buffer = ft_strjoin(buffer, char_buf);
			buffer = remove_file_name(buffer);
			g = 0;
			while (buffer[g])
			{
				if (buffer[g] == '\n' && !buffer[g + 1])
					buffer[g] = '\0';
				g++;
			}
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
	free(buffer);
	free(char_buf);
	free(formatted);
	return (hd_fd);
}


// int	main(int ac, char **av, char **envp)
//  {
//  	char *s = "$(ls) nb: $(env | grep CODE | wc -l)";
//  	int	hd_fd;
// 	char	formatted[500];
//  	char *dup;
//  	t_list	**env;
 
//  	env = lst_env(envp);
//  	dup = ft_strdup(s);
//  	hd_fd = format_here_doc(dup, env, envp);
// 	// hd_fd = open(HD_TEMP_FILE, O_RDONLY);
// 	read(hd_fd, formatted, 500);
//  	printf("%s\n", formatted);
//  	// printf("%s\n", s);
//  	(void)ac;
//  	(void)av;
//  }