/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:55:51 by secros            #+#    #+#             */
/*   Updated: 2025/04/18 10:05:03 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_user_in_pwd(void)
{
	char	*res;
	char	pwd[PATH_MAX];
	int		i;
	int 	j;

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
		res = fill_dishwasher(ft_substr(pwd, i, i +j), free, get_sink(NULL));
	}
	return (res);
}
