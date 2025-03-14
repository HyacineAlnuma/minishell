/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:14:43 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/19 13:45:13 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../includes/minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	int		i;
	int		s_len;
	char	*res;

	s_len = ft_strlen(s);
	res = malloc(sizeof(char) * s_len + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}


char	*format_here_doc(char *str, t_list **env)
{
	int		i;
	int		j;
	char	*cmd;
	char	*formated;
	int		size;
	t_exec	**cmds;

	i = -1;
	j = 0;
	size = 0;
	while (str[++i])
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			size += (i - 1);
			j = i;
			while (str[++j])
			{
				if (str[j] == ')')
					break ;
			}
			if (j == ft_strlen(str))
			{
				perror("parse error");
				return (NULL);
			}
			// printf("%d\n", i);
			// printf("%c\n", str[j]);
			str[j] = '\0';
			cmd = &str[i + 2];
			i = j + 1;
			cmds = parsing(cmd, env);
		}
	}
	return (cmd);
}

int	main(int ac, char **av, char **envp)
{
	char *s = "yo la $(ls) team $(cd) yoo\n";
	char *cmd;
	char *dup;
	t_list	**env;

	env = lst_env(envp);
	dup = ft_strdup(s);
	cmd = format_here_doc(dup, env);
	printf("%s\n", cmd);
	printf("%s\n", s);
}