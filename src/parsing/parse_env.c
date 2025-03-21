/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:15 by secros            #+#    #+#             */
/*   Updated: 2025/03/21 09:29:01 by halnuma          ###   ########.fr       */
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
		if (str[i] == '$' && ft_isalnum(str[i + 1]))
		{
			if (ft_isdigit(str[i + len]))
			{
				while (ft_isdigit(str[i + len]))
					len++;
				return (handle_env(replace_env(str, env, i, len), env));
			}
			while (str[i + len] && ft_isalpha(str[i + len]))
				len++;
			return (handle_env(replace_env(str, env, i, len), env));
		}
		i++;
	}
	return (str);
}
