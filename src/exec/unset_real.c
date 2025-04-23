/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_real.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:32:35 by secros            #+#    #+#             */
/*   Updated: 2025/04/23 11:35:59 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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