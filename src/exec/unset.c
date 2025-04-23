/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:32:35 by secros            #+#    #+#             */
/*   Updated: 2025/04/23 11:56:20 by secros           ###   ########.fr       */
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
				return (1);
			}
		}
		ptr = ptr->next;
	}
	return (1);
}

char	*find_env(t_list **env, char *var)
{
	t_list	*ptr;
	size_t	size;
	char	*ref;

	ptr = *env;
	size = ft_strlen(var);
	if (!size || var[size - 1] == '=')
		return (NULL);
	while (ptr)
	{
		ref = ft_strnstr(ptr->content, var, size);
		if (ref && (!ref[size] || ref[size] == '='))
			return (ref);
		ptr = ptr->next;
	}
	return (NULL);
}

int	unset(t_exec *cmd, t_list **env, int index)
{
	int		i;
	char	*ref;

	i = 1;
	(void) index;
	while (cmd->opt[i])
	{
		ref = find_env(env, cmd->opt[i]);
		i++;
		ft_lst_hand_wash_if(env, ref, compare, get_sink(NULL));
	}
	return (0);
}
