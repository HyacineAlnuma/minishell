/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:15 by secros            #+#    #+#             */
/*   Updated: 2025/04/25 14:40:36 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			while (((char *)lst->content)[i]
				&& ((char *)lst->content)[i] != '=')
				i++;
			free (var_env);
			if (((char *)lst->content)[i])
				return (&((char *)lst->content)[i + 1]);
			return (NULL);
		}
		lst = lst->next;
	}
	free (var_env);
	return (NULL);
}

char	*add_return_status(char *str)
{
	char	*exit_status;

	exit_status = ft_itoa(last_status_code(0, 1));
	if (!exit_status)
		return (NULL);
	str = ft_strjoin(str, exit_status);
	free(exit_status);
	return (str);
}

char	*replace_env(char *str, t_list **env, t_vect pos, t_sink **bin)
{
	char	*var_env;
	char	*new_str;

	var_env = fill_dishwasher(ft_substr(str, pos.x, pos.y), free, bin);
	if (!var_env)
		return (NULL);
	new_str = fill_dishwasher(ft_substr(str, 0, pos.x), free, bin);
	if (!new_str)
		return (NULL);
	if (var_env[1] == '?')
		new_str = fill_dishwasher(add_return_status(new_str), free, bin);
	else
		new_str = fill_dishwasher(ft_strjoin(new_str, find_node(env, var_env)), \
	free, bin);
	if (!new_str)
		return (NULL);
	new_str = fill_dishwasher(ft_strjoin(new_str, (str + pos.x + pos.y)), \
	free, bin);
	return (new_str);
}

char	*digit_env(char *str, t_list **env, t_sink **bin, size_t i)
{
	size_t	len;

	len = 1;
	while (ft_isdigit(str[i + len]))
		len++;
	return (handle_env(replace_env(str, env, \
	(t_vect){i, len}, bin), env, bin));
}

char	*handle_env(char *str, t_list **env, t_sink **bin)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (str && str[i])
	{
		len = 1;
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '?'
				|| str[i + 1] == '_'))
		{
			if (ft_isdigit(str[i + len]))
				return (handle_env(replace_env(str, env, \
					(t_vect){i, len + 1}, bin), env, bin));
			if (str[i + len] == '?')
				return (handle_env(replace_env(str, env, \
					(t_vect){i, len + 1}, bin), env, bin));
			while (str[i + len] && (ft_isalnum(str[i + len])
					|| str[i + len] == '_'))
				len++;
			return (handle_env(replace_env(str, env, \
			(t_vect){i, len}, bin), env, bin));
		}
		i++;
	}
	return (str);
}
