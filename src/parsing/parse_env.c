/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:15 by secros            #+#    #+#             */
/*   Updated: 2025/04/09 10:27:19 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_user_in_pwd(void)
{
	char	*res;
	char	pwd[PATH_MAX];
	int		i;

	if (getcwd(pwd, sizeof(pwd)) == NULL)
	{
		perror("getcwd() error");
		return (NULL);
	}
	else
	{
		i = 1;
		while (pwd[i] != '/')
			i++;
		res = &pwd[i + 1];
		i++;
		while (pwd[i] != '/')
			i++;
		pwd[i] = '\0';
	}
	return (res);
}

char	*find_node(t_list **env, char *var_env)
{
	size_t	i;
	t_list	*lst;

	i = 0;
	lst = *env;
	var_env = ft_strjoin(var_env, "=");
	if (!var_env)
		return (NULL);
	while (lst)
	{
		if (!ft_strcmp((char *)lst->content, var_env + 1))
		{
			while (((char *)lst->content)[i] != '=')
				i++;
			free (var_env);
			return (&((char *)lst->content)[i + 1]);
		}
		lst = lst->next;
	}
	free (var_env);
	return (NULL);
}

//Ca va etre fun a securiser !
char	*replace_env(char *str, t_list **env, t_vect pos, t_sink *bin)
{
	char	*var_env;
	char	*new_str;

	var_env = fill_dishwasher(ft_substr(str, pos.x, pos.y), free, &bin);
	if (!var_env)
		return (NULL);
	new_str = fill_dishwasher(ft_substr(str, 0, pos.x), free, &bin);
	if (!new_str)
		return (NULL);
	new_str = fill_dishwasher(ft_strjoin(new_str, find_node(env, var_env)), \
	free, &bin);
	if (!new_str)
		return (NULL);
	new_str = fill_dishwasher(ft_strjoin(new_str, (str + pos.x + pos.y)), \
	free, &bin);
	return (new_str);
}

char	*handle_env(char *str, t_list **env, t_sink *bin)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (str && str[i])
	{
		len = 1;
		if (str[i] == '$' && ft_isalnum(str[i + 1]))
		{
			if (ft_isdigit(str[i + len]))
			{
				while (ft_isdigit(str[i + len]))
					len++;
				return (handle_env(replace_env(str, env, \
				(t_vect){i, len}, bin), env, bin));
			}
			while (str[i + len] && ft_isalnum(str[i + len]))
				len++;
			return (handle_env(replace_env(str, env, \
			(t_vect){i, len}, bin), env, bin));
		}
		i++;
	}
	return (str);
}
