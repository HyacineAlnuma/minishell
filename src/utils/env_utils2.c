/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:55:51 by secros            #+#    #+#             */
/*   Updated: 2025/04/11 13:56:51 by secros           ###   ########.fr       */
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
