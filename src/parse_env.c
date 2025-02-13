/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:15 by secros            #+#    #+#             */
/*   Updated: 2025/02/13 10:52:49 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_node(t_list **env, char *var_env)
{
	size_t	i;
	t_list	*lst;

	i = 0;
	lst = *env;
	if (!var_env)
		return (NULL);
	var_env = ft_strjoin(var_env, "=");
	while (lst)
	{
		if (!ft_strcmp((char *)lst->content, var_env + 1))
		{
			while (((char *)lst->content)[i] != '=')
				i++;
			return (&((char *)lst->content)[i + 1]);
		}
		lst = lst->next;
	}
	return (NULL);
}

char	*handle_env(char *str, t_list **env)
{
	size_t	i;
	size_t	len;
	char	*var_env;
	char	*new_str;

	(void)env;
	i = 0;
	while (str[i])
	{
		len = 0;
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			while (str[i + len] && str[i + len] != ' ' && str[i + len] != '/')
				len++;
			var_env = ft_substr(str, i, len); //Ca va etre fun a securiser !
			new_str = ft_substr(str, 0, i);
			new_str = ft_strappend(new_str, find_node(env, var_env));
			new_str = ft_strappend(new_str, (str + i + len));
			free(str);
			return (new_str);
		}
		i++;
	}
	return (str);
}

t_list	**lst_env(char **envp)
{
	int		i;
	t_list	**lst_env;
	t_list	*new_line;

	i = 0;
	lst_env = (t_list **)ft_calloc(sizeof(t_list **), 1);
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
