
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:32:35 by secros            #+#    #+#             */
/*   Updated: 2025/04/23 10:56:20 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calc_var_size(char *var)
{
	int		var_size;

	var_size = 0;
	while (var[var_size] && var[var_size] != '=')
		var_size++;
	return (var_size);
}


int	unset_exp(char *var, t_list **env)
{
	t_list	*ptr;
	int		var_size;
	int		env_var_size;

	var_size = calc_var_size(var);
	ptr = *env;
	while (ptr)
	{
		env_var_size = calc_var_size(ptr->content);
		if (env_var_size == var_size)
		{
			if (!ft_strncmp(ptr->content, var, var_size))
			{
				if (var[var_size] != '=')
					return (0);
				ft_lst_remove_if(env, ptr->content, compare, 0);
			}
		}
		ptr = ptr->next;
	}
	return (1);
}
