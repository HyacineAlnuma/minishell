/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:05:15 by secros            #+#    #+#             */
/*   Updated: 2025/03/19 09:36:17 by halnuma          ###   ########.fr       */
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

t_list	**get_small_env(t_list	**env)
{
	t_list	*new_line;
	char	pwd[PATH_MAX];
	char	*env_pwd;

	if (getcwd(pwd, sizeof(pwd)) == NULL)
	{
		perror("getcwd() error");
		return (NULL);
	}
	else
	{
		env_pwd = ft_strjoin("PWD=", pwd);
		if (!env_pwd)
			return (NULL);
		new_line = ft_lstnew(env_pwd);
		if (!new_line)
			return (NULL);
		ft_lstadd_back(env, new_line);
		new_line = ft_lstnew("SHLVL=1");
		if (!new_line)
			return (NULL);
		ft_lstadd_back(env, new_line);
		new_line = ft_lstnew("_=/usr/bin/env");
		if (!new_line)
			return (NULL);
		ft_lstadd_back(env, new_line);
		return (env);
	}
}

char	*increment_shlvl(char *str)
{
	char	*res;
	char	*s_nb;
	int		i_nb;
	int		i;

	i = 0;
	while (str[i] != '=')
		i++;
	s_nb = &str[i + 1];
	i_nb = ft_atoi(s_nb);
	i_nb++;
	s_nb = ft_itoa(i_nb);
	if (!s_nb)
		return (NULL);
	res = ft_strjoin("SHLVL=", s_nb);
	if (!res)
		return (NULL);
	return (res);
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
	if (!envp[0])
	{
		lst_env = get_small_env(lst_env);
		if (!lst_env)
			return (NULL);
		return (lst_env);
	}
	while (envp[i])
	{
		if (!strncmp(envp[i], "SHLVL=", 6))
			envp[i] = increment_shlvl(envp[i]);
		new_line = ft_lstnew(envp[i]);
		if (!new_line)
			return (NULL);
		ft_lstadd_back(lst_env, new_line);
		i++;
	}
	return (lst_env);
}
