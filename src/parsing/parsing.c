/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/03/17 16:14:59 by secros           ###   ########.fr       */
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

char	*remove_quote(char *str)
{
	char	*tmp;

	if (!str)
		return (NULL);
	if (str[0] == '\'' || str[0] == '"')
	{
		tmp = ft_substr(str, 1, ft_strlen(str) - 2);
		free(str);
		if (tmp[0] == '\0')
		{
			free(tmp);
			tmp = NULL;
		}
		str = tmp;
	}
	return (str);
}

int	env_handling(t_list *tokens, t_list **env)
{
	char	*token;

	while (tokens)
	{
		token = (char *)tokens->content;
		if (token && token[0] != '\'')
			tokens->content = handle_env(token, env);
		tokens = tokens->next;
	}
	return (1);
}


void	cut_pipe(t_list **tokens)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = NULL;
	while (*tokens)
	{
		if ((*tokens)->content && !strcmp((char *)(*tokens)->content, "|"))
		{
			tmp = *tokens;
			break ;
		}
		prev = *tokens;
		*tokens = (*tokens)->next;
	}
	if (*tokens)
	{
		(*tokens) = (*tokens)->next;
		prev->next = NULL;
	}
	if (tmp)
		ft_lstdelone(tmp, free);
}

t_list	**cut_instruction(t_list *tokens, int count)
{
	t_list	**pipe;
	int		i;

	i = 0;
	
	pipe = ft_calloc(sizeof(t_list *), (count + 2));
	if (!pipe)
		return (NULL);
	while (i <= count)
	{
		pipe[i++] = tokens;
		cut_pipe(&tokens);
	}
	return (pipe);
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

enum e_doc	find_type(char *str)
{
	// add an error check if invalid redirection like strlen > 2 or bad redirection
	if (!strncmp(str, "<\0", 2))
		return (INFILE);
	if (!strcmp(str, "<<\0"))
		return (HEREDOC);
	if (!strncmp(str, ">\0", 2))
		return (OUTFILE);
	if (!strcmp(str, ">>\0"))
		return (APPEND);
	return (0);
}

t_doc	polish_doc(t_list **lst, t_list *tmp)
{
	t_doc	current;
	t_list	*to_free;

	current.type = find_type((char *)(*lst)->content);
	// ft_printf("Type [%d]", current.type);
	(*lst) = (*lst)->next->next;
	merge_tokens(*lst);
	current.str = (*lst)->content;
	// ft_printf(" Name [%s]\n", current.str);
	clear_to(tmp, *lst);
	to_free = *lst;
	*lst = (*lst)->next;
	tmp->next = *lst;
	free(to_free);
	return (current);
}

t_doc *create_docs(t_list *lst)
{
	t_doc	*docs;
	t_list	*tmp;
	int		i;

	i = lst_count_char(lst, '<') + lst_count_char(lst, '>');
	docs = ft_calloc(sizeof(t_doc), i + 1);
	if (!docs)
		return (NULL);
	tmp = lst;
	i = 0;
	while (lst)
	{
		if (lst->content && (!ft_strcmp(lst->content, "<") \
		|| !ft_strcmp(lst->content, ">")))
			docs[i++] = polish_doc(&lst, tmp);
		if (!lst)
			break ;
		tmp = lst;
		lst = lst->next;
	}
	return (docs);
}

int compare(char *str, char *str_ref)
{
	if (!str || !str_ref)
	{
		if (!str && !str_ref)
			return (0);
		else
			return (1);
	}
	return (ft_strcmp(str, str_ref));
}

void	merge_all(t_list *lst)
{
	while (lst)
	{
		merge_tokens(lst);
		lst = lst->next;
	}
}

/* void	clear_lsts(t_list **lsts)
{
	int	i;

	i = 0;
	while (lsts[i])
	{
		ft_lstclear(&lsts[i], free);
	}
	free(lsts);
} */

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

void	destroy_doc(void *pt)
{
	t_doc	*docs;
	size_t	i;

	i = 0;
	docs = pt;
	while(docs[i].str)
	{
		free(docs->str);
		i++;
	}
	free_the_mallocs(pt);
}

t_exec	**parsing(char *str, t_list **env, t_list **bin)
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
	i = 0;
	if (!str)
		return (NULL);
	tokens = create_token_list(str);
	if (!tokens)
		return (NULL);
	if (!tokens)
		return (NULL);
	env_handling(tokens, env);
	count = lst_count_char(tokens, '|');
	piped = cut_instruction(tokens, count);
	exec = ft_calloc(sizeof(t_exec *), count + 2);
	while (i <= count)
	{
		exec[i] = ft_calloc(sizeof(t_exec), 1);
		exec[i]->docs = create_docs(piped[i]);
		add_garbage(exec[i]->docs, destroy_doc, bin);
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
		ft_lstclear(&piped[i], NULL);
		add_garbage(tab, free_the_mallocs, bin);
		exec[i]->cmd = tab[0];
		exec[i]->opt = tab;
		i++;
	}
	free (piped);
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
