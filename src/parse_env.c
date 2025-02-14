/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:15 by secros            #+#    #+#             */
/*   Updated: 2025/02/14 13:37:54 by secros           ###   ########.fr       */
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

char	*replace_env(char *str, t_list **env, size_t index, size_t len)
{
	char	*var_env;
	char	*new_str;

	var_env = ft_substr(str, index, len); //Ca va etre fun a securiser !
	new_str = ft_substr(str, 0, index);
	new_str = ft_strappend(new_str, find_node(env, var_env));
	new_str = ft_strappend(new_str, (str + index + len));
	free(str);
	return (new_str);
}

char	*handle_env(char *str, t_list **env)
{
	size_t	i;
	size_t	len;

	(void)env;
	i = 0;
	while (str[i])
	{
		len = 1;
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			if (str[i + len] >= '0' && str[i + len] <= '9')
			{
				while (str[i + len] && str[i + len] >= '0' && str[i + len] <= '9')
					len++;
				return (handle_env(replace_env(str, env, i, len), env));
			}
			while (str[i + len] && str[i + len] != ' ' && str[i + len] != '/' && str[i + len] != '$')
				len++;
			return (handle_env(replace_env(str, env, i, len), env));
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
