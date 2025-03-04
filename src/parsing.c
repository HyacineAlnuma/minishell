/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/03/04 17:27:11 by secros           ###   ########.fr       */
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
			Handle pipe first before they could get unquoted OK pipe don't get unquoted (except """|")
		unify token ex (He"lo" -> Hello) only the one with no whitespace in between OK
		if token = '|' handle later in the parsing
			cut lst in two before | after
		repeat for the after part
		handle env variable and quote
		remove quote;
	create the struct
		(first element of lst) = 1
		handle redirection (and heredoc)
			set token to infile / outfile
		if first element token = command else token = option
		if token = pipe
			new_struct and repeat with the other lst
	when lst = null
		parsing end
		destroy the lst but keep content;
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

t_exec	*create_exec(t_list *tokens)
{
	int		first;
	t_exec 	*exec;
	char	**opt;
	int		j = 0;

	first = 1;
	exec = ft_calloc(sizeof(t_exec), 1);
	opt = ft_calloc(sizeof(char *), 64);
	while (tokens)
	{
		if (ft_strcmp((char *)tokens->content, "|"))
			return NULL; //should create another struct
		else if (ft_strcmp((char *)tokens->content, ">>"))
			exec->outfile = tokens->content;
		else if (ft_strcmp((char *)tokens->content, ">"))
			exec->outfile = tokens->content;
		else if (ft_strcmp((char *)tokens->content, "<<"))
			return NULL;
		else if (ft_strcmp((char *)tokens->content, "<"))
			exec->infile = (char *)tokens->content;
		else if (first == 1)
		{
			first = 0;
			exec->cmd = (char *)tokens->content;
		}
		else
			opt[j++] = (char *)tokens->content;
		tokens = tokens->next;
	}
	exec->opt = opt;
	return (exec);
}

t_exec	**parsing_v2(char *str)
{
	size_t	i;
	t_list	*tokens;
	t_list	*new;
	char	*token;
	t_exec 	**exec;

	i = 0;
	exec = ft_calloc(sizeof(t_exec *), 2);
	tokens = NULL;
	while (str[i])
	{
		token = split_token(str, &i);
		while (str[i] && ((is_redir(str[i]) && is_redir(str[i - 1])) || \
		((!is_space(str[i]) && !is_redir(str[i])) && (!is_space(str[i - 1]) && \
		!is_redir(str[i - 1])))))
			token = ft_strappend(remove_quote(token), remove_quote(split_token(str, &i)));
		new = ft_lstnew(token);
		ft_lstadd_back(&tokens, new);
	}
	exec[0] = create_exec(tokens);
	while (tokens)
	{
		ft_printf("[%s]\n", (char *)tokens->content);
		tokens = tokens->next;
	}
	return (exec);
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
		command = parsing_v2(input);
		if (command)
			exec(command, env, envp);
	}
}
