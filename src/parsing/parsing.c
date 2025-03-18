/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/03/18 13:32:18 by secros           ###   ########.fr       */
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

	if (!str)
		return (NULL);
	tokens = create_token_list(str, bin);
	if (!tokens)
		return (NULL);
	env_handling(tokens, env, bin);
	count = lst_count_char(tokens, '|');
	piped = add_garbage(cut_instruction(tokens, count), free, &bin);
	exec = ft_malloc((sizeof(t_exec *) * (count + 2)), &bin);
	ft_bzero(exec, (sizeof(t_exec *) * (count + 2)));
	i = 0;
	while (i <= count)
	{
		exec[i] = add_garbage(ft_calloc(sizeof(t_exec), 1), free, &bin);
		exec[i]->docs = create_docs(piped[i], bin);
		exec[i]->bin = bin;
		merge_all(piped[i], bin);
		ft_lst_ft_free_if(&piped[i], NULL, compare, bin);
		tab = add_garbage(ft_calloc(sizeof(char *), lst_len(piped[i]) + 1), free, &bin);
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
	while (str[i] && HD_TEMP_FILE[i])
	{
		if (str[i] != HD_TEMP_FILE[i])
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
	char	*formated;
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
	formated = NULL;
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
			formated = ft_strappend(formated, &str[begin_part]);
			cmd = ft_strndup(&str[i + 2], (j - (i + 2)));
			i = j + 1;
			begin_part = i;
			cmds = parsing(cmd, env, NULL);
			k = -1;
			while (cmds[++k])
				cmds[k]->here_doc = 1;
			exec(cmds, env, envp);
			temp_file_fd = open(HD_TEMP_FILE, O_RDONLY);
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
			unlink(HD_TEMP_FILE);
			formated = ft_strappend(formated, buffer);
		}
		i++;
	}
	formated = ft_strappend(formated, &str[begin_part]);
	hd_fd = open(HD_TEMP_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	f_len = ft_strlen(formated);
	write(hd_fd, formated, f_len);
	free(buffer);
	free(char_buf);
	free(formated);
	return (hd_fd);
}
