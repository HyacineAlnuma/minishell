/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:55:51 by secros            #+#    #+#             */
/*   Updated: 2025/04/18 17:41:40 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_user_in_pwd(void)
{
	char	*res;
	char	pwd[PATH_MAX];
	int		i;
	int		j;

	j = 0;
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
		while (pwd[i + j] != '/')
			j++;
		i++;
		res = fill_dishwasher(ft_substr(pwd, i, i + j), free, get_sink(NULL));
	}
	return (res);
}

void	relink_lst(t_list **prev, t_list **head, t_list **lst)
{
	if (*prev)
		(*prev)->next = *lst;
	else
		*head = *lst;
}

void	print_exp_env(t_list **env)
{
	t_list	*ptr;
	t_list	**alpha_env;
	char	*var_env;

	alpha_env = get_alpha_env(env);
	if (!alpha_env)
		return ;
	ptr = *alpha_env;
	while (ptr)
	{
		var_env = (char *)ptr->content;
		ft_printf("declare -x ");
		print_quoted_env(var_env);
		ptr = ptr->next;
	}
	ft_lstclear(alpha_env, NULL);
}
