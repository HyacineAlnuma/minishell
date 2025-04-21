/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:32:35 by secros            #+#    #+#             */
/*   Updated: 2025/04/21 14:15:47 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calc_var_size(t_exec *cmd, int i, int *have_equal)
{
	int		var_size;

	var_size = 0;
	while (cmd->opt[i][var_size] && cmd->opt[i][var_size] != '=')
		var_size++;
	if (cmd->opt[i][var_size])
		var_size++;
	else
		have_equal = 0;
	return (var_size);
}

char	*find_data_ref(t_list **env, t_exec *cmd, int i, int var_size)
{
	char	*data_ref;
	t_list	*ptr;

	ptr = *env;
	data_ref = NULL;
	while (ptr)
	{
		if (!ft_strncmp(ptr->content, cmd->opt[i], var_size))
		{
			data_ref = ptr->content;
			break ;
		}
		ptr = ptr->next;
	}
	return (data_ref);
}

int	unset_var(char *data_ref, int have_equal, int index, t_list **env)
{
	if (data_ref && (have_equal || !index))
	{
		ft_lst_remove_if(env, data_ref, ft_strcmp, 0);
		if (index)
			return (1);
	}
	else if (data_ref)
		return (2);
	return (0);
}

int	unset(t_exec *cmd, t_list **env, int index)
{
	char	*data_ref;
	int		var_size;
	int		i;
	int		have_equal;
	int		unset_val;

	i = 1;
	unset_val = 0;
	have_equal = 1;
	if (!cmd->opt[1])
		return (0);
	if (index)
		i = index;
	while (cmd->opt[i])
	{
		var_size = calc_var_size(cmd, i, &have_equal);
		data_ref = find_data_ref(env, cmd, i, var_size + 1);
		unset_val = unset_var(data_ref, have_equal, index, env);
		if (unset_val)
			return (unset_val);
		if (index)
			break ;
		i++;
	}
	return (1);
}
