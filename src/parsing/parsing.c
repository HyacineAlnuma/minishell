/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/03/13 17:41:18 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "color.h"

/* 
	create the struct
		(first element of lst) = 1
		handle redirection (and heredoc)
			set token to infile / outfile
		if first element token = command else token = option
		if token = pipe OK
			new_struct and repeat with the other lst OK
	when lst = null
		parsing end
		destroy the lst but keep content; lst_clear with NULL
		send strut to exect

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
	ft_printf("Type [%d]", current.type);
	(*lst) = (*lst)->next->next;
	merge_tokens(*lst);
	current.str = (*lst)->content;
	ft_printf(" Name [%s]\n", current.str);
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
	return (NULL);
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

t_list	*parsing(char *str, t_list **env)
{
	t_list	**piped;
	t_list	*tokens;
	int		count;
	
	tokens = create_token_list(str);
	env_handling(tokens, env);
	count = lst_count_char(tokens, '|');
	piped = cut_instruction(tokens, count);
	create_docs(piped[0]);
	ft_lst_remove_if(&piped[0], NULL, compare);
	/*	Next step is to change environnement handling to iter over each element of the list OK
		cut lst based on pipe OK
		create t_doc tab
		then remove quote tokens OK
		then merge token into final token OK
	*/
	print_lsts(piped);
	return (NULL);
}
