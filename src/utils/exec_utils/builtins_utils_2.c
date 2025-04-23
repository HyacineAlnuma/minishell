/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:46:07 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/23 09:43:22 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_previous_pwd(t_list **env)
{
	char	*previouspwd;
	char	*str;
	int		i;
	t_list	*ptr;

	ptr = *env;
	previouspwd = NULL;
	while (ptr)
	{
		if (!ft_strncmp(ptr->content, "OLDPWD=", 7))
		{
			i = -1;
			str = ptr->content;
			while (str[++i])
			{
				if (str[i] == '=')
					break ;
			}
			previouspwd = &str[++i];
		}
		ptr = ptr->next;
	}
	if (!previouspwd)
		last_status_code(1, 2);
	return (previouspwd);
}
