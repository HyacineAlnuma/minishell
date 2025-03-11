/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/03/11 11:11:32 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "color.h"

static int	synthax_error(char *str)
{
	size_t	j;
	int		i;

	j = 0;
	i = 0;
	while (str && str[j])
	{
		i = 1;
		while (str[j] && str[j] != '|')
		{
			if (!(str[j] == ' ' || (str[j] >= 9 && str[j] <= 13)))
				i = 0;
			j++;
		}
		if (str[j] && str[j] == '|')
		{
			j++;
			i++;
		}
		if (i > 1)
			return (write (2, "zsh: parse count near `|'\n", 27), 1);
	}
	if (i == 1)
		return (2);
	return (0);
}

void	finish_sentence(char **str, char c)
{
	char	*next;
	char	*quote;

	next = NULL;
	quote = NULL;
	while (!next || !ft_strchr(next, c))
	{
		free(next);
		if (c == '\'')
			next = readline("quote>");
		if (c == '"')
			next = readline("dquote>");
		if (c == '|')
			next = readline("pipe>");		
		if (c == '"' || c == '\'')
			quote = ft_strappend(quote, "\n");
		quote = ft_strappend(quote, next);
	}
	*str = ft_strappend(*str, quote);
	free(quote);
	// return (str);
}

int	pipe_error()
{
	write (2, "zsh: parse count near `|'\n", 27);
	return (FALSE);
}

int	pipe_check(char *str, size_t i, char *c)
{
	size_t	j;

	j = i;
	while (str[j])
	{
		if (!(str[j] == ' ' || (str[j] >= 9 && str[j] <= 13)))
			break ;
		if (j == 0 || str[j] == '|')
			return (pipe_error());
		j--;
	}
	while (str[i])
	{
		if (!(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
			return (TRUE);
		if (str[i] == '|')
			return (pipe_error());
		i++;
	}
	finish_sentence(&str, *c);
	return (1);
}

char	*synthax_quote(char *str)
{
	size_t	i;
	char	c;

	i = 0;
	c = 0;
	while (str && str[i])
	{
		c = 0;
		if (c == 0 && (str[i] == '"' || str[i] == '\'' || str[i] == '|'))
			c = str[i++];
		if (c == '|' && !pipe_check(str, i, &c))
			return (NULL);
		if ((c == '\'' || c == '"') && !ft_strchr(&str[i], c))
			finish_sentence(&str, c);
		while (str[i] != c && (c != 0 || c == '|'))
			i++;
		i++;
	}
	return (str);
}

char	***divide_arg(char *str)
{
	char	***command;
	char	**args;
	size_t	tablen;

	args = ft_split(str, '|');
	if (!args)
		return (NULL);
	tablen = ft_tablen(args);
	command = ft_calloc(sizeof(char **), tablen + 1);
	if (!command)
	{
		free_the_mallocs((void **) args);
		return (NULL);
	}
	while (tablen--)
	{
		command[tablen] = ft_multi_split(args[tablen], CHARSET);
		if (!command[tablen])
			free_the_mallocs((void **) command);
	}
	free_the_mallocs((void **) args);
	free(str);
	return (command);
}

t_exec	**parsing(char *str, t_list **env)
{
	size_t	count;
	t_exec	**command;
	char	*tmp;

	count = synthax_error(str);
	if (!str || count == 1)
		return (free(str), NULL);
	else if (count == 2)
	{
		tmp = readline("pipe> ");
		str = ft_strappend(str, tmp);
		free(tmp);
		return (parsing(str, env));
	}
	add_history(str);
	count = count_char(str, '|');
	str = handle_env(str, env);
	command = create_struct(divide_arg(str), count + 1);
	return (command);
}

void	print_prompt(t_list **env)
{
	char	*prompt;

	prompt = find_node(env, "$USER");
	ft_printf("%s%s%s ", BG_RED, prompt, RESET);
	prompt = find_node(env, "$PWD");
	ft_printf("%s%s%s%s\n", BOLD, FG_BRIGHT_BLUE, prompt, RESET);
}

/* New parsing in progress
	New system, string cut down in token separate by whitespace or quote or pipe OK
		Need to separete pipe also OK
			Handle pipe first before they could get unquoted OK
		
		unify token ex (He"lo" -> Hello) only the one with no whitespace in between OK
		if token = '|' handle later in the parsing OK
			cut lst in two before | after OK
		repeat for the after part OK
		handle env variable and quote OK
		remove quote; OK
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

int	is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	is_redir(char c)
{
	if (c == '>' || c == '<' || c =='|')
		return (1);
	return (0);
}

char	*split_token(char *str, size_t *i)
{
	unsigned int	count;
	char			*token;
	char			quote;

	count = 0;
	quote = 0;
	while (is_space(str[*i]))
		*i += 1;
	if (str[*i] == '"' || str[*i] == '\'')
		quote = str[*i + count++];
	while(str[*i + count] && (!is_space(str[*i + count]) || quote))
	{
		if (str[*i + count] == quote || (!quote && is_redir(str[*i])))
		{
			count++;
			break ;
		}
		else if (!quote && (is_redir(str[*i + count]) \
		|| str[*i + count] == '"' || str[*i + count] == '\''))
			break ;
		count++;
	}
	token = ft_substr(str, *i, count);
	*i += count;
	return (token);
}

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

void	*add_empty(t_list **lst)
{
	t_list	*new;

	new = ft_lstnew(NULL);
	if (!new)
		return (NULL);
	ft_lstadd_back(lst, new);
	return (new);
}

void	clear_to(t_list	*start, t_list *end)
{
	t_list	*tmp;
	t_list	*next;

	tmp = start->next;
	while (tmp && tmp != end)
	{
		next = tmp->next;
		ft_lstdelone(tmp, free);
		tmp = next;
	}
	start->next = end;
}

int	merge_tokens(t_list **tokens)
{
	t_list	*tmp;
	t_list	*prev;
	char	*new_token;

	tmp = *tokens;
	while (tmp)
	{
		new_token = NULL;
		prev = tmp;
		while (tmp && tmp->content)
		{
			tmp->content = remove_quote((char *)tmp->content);
			new_token = ft_strappend(new_token, (char *)tmp->content);
			tmp = tmp->next;
		}
		while (tmp && !tmp->content)
			tmp = tmp->next;
		prev->content = new_token;
		clear_to(prev, tmp);
	}
	return (1);
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

int	count_pipe(t_list *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->content && !strcmp((char *)tokens->content, "|"))
			count++;
		tokens = tokens->next;
	}
	return (count);
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

/* t_exec	*create_struct2(t_list	*tokens)
{
	t_exec	*l_com;

	l_com = malloc(sizeof(t_exec));
	if (!l_com)
		return (NULL);
	return (l_com);
} */

t_list	**cut_instruction(t_list *tokens)
{
	t_list	**pipe;
	t_exec	**com;
	int		count;
	int		i;

	i = 0;
	count = count_pipe(tokens);
	pipe = malloc(sizeof(t_list *) * (count + 1));
	if (!pipe)
		return (NULL);
	com = ft_calloc(sizeof(t_exec *), (count + 2));
	if (!com)
		return (NULL);
	while (i <= count)
	{
		pipe[i++] = tokens;
		cut_pipe(&tokens);
	}
	while (--i >= 0)
		merge_tokens(&pipe[i]);
	return (pipe);
}

t_list	*parsing_v2(char *str, t_list **env)
{
	size_t	i;
	t_list	*tokens;
	t_list	*new;
	t_list	**piped;
	char	*token;

	i = 0;
	tokens = NULL;
	while (str[i])
	{
		token = split_token(str, &i);														//TODO	Maybe change the tokens to be separate by space tokens [ec]["h"][o][ ][...]
		new = ft_lstnew(token);																//Idea add space token between instruction to merge token later
		if (!token || !new)																	//This is for environnemnt handling (STATUS : NOT DONE)
			return (ft_lstclear(&tokens, free), NULL);
		ft_lstadd_back(&tokens, new);
		if (str[i] && (is_space(str[i]) || (is_redir(str[i]) && \
		!is_redir(str[i - 1])) || (is_redir(str[i - 1]) && !is_redir(str[i]))))
			add_empty(&tokens);
	}
	env_handling(tokens, env);
	piped = cut_instruction(tokens);
	/*	Next step is to change environnement handling to iter over each element of the list OK
		cut lst based on pipe OK
		create t_doc tab
		then remove quote tokens OK
		then merge token into final token OK
	*/
	while (piped[0])
	{
		ft_printf("[%s]\n", (char *)piped[0]->content);
		piped[0] = piped[0]->next;
	}
	while (piped[1])
	{
		ft_printf("[%s]\n", (char *)piped[1]->content);
		piped[1] = piped[1]->next;
	}
	while (piped[2])
	{
		ft_printf("[%s]\n", (char *)piped[2]->content);
		piped[2] = piped[2]->next;
	}
	return (NULL);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_list	**env;
	t_exec	**command;

	(void) av;
	if (ac != 1)
	{
		write (2, "Error\nBad arguments\n", 20);
		fflush(stderr);
		return (1);
	}
	print_ascii();
	command = NULL;
	env = lst_env(envp);
	while (1)
	{
		print_prompt(env);
		input = readline("hell % ");
		if (!input)
			break ;
		// input = synthax_quote(input);
		// ft_printf("%s\n", input);
		// command = parsing(input, env);
		parsing_v2(input, env);
		if (command)
			exec(command, env, envp);
	}
}
