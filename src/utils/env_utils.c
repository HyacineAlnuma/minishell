/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:18:38 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/21 10:27:44 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**lst_to_tab(t_list **lst)
{
	char	**tab;
	t_list	*ptr;
	int		lst_size;
	int		i;

	lst_size = ft_lstsize(*lst);
	tab = (char **)malloc(sizeof(char *) * (lst_size + 1));
	i = 0;
	ptr = *lst;
	while (ptr)
	{
		tab[i] = (char *)ptr->content;
		ptr = ptr->next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

t_list	**build_small_env(t_list **env, char *env_pwd)
{
	t_list	*new_line;

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

t_list	**get_small_env(t_list	**env)
{
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
		return (build_small_env(env, env_pwd));
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

	i = -1;
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
	while (envp[++i])
	{
		if (!strncmp(envp[i], "SHLVL=", 6))
			envp[i] = increment_shlvl(envp[i]);
		new_line = ft_lstnew(envp[i]);
		if (!new_line)
			return (NULL);
		ft_lstadd_back(lst_env, new_line);
	}
	return (lst_env);
}
