/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:35:19 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/18 16:02:16 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtins(char *cmd)
{
	if (!ft_strncmp(cmd, "echo", 5))
		return (1);
	else if (!ft_strncmp(cmd, "cd", 3))
		return (1);
	else if (!ft_strncmp(cmd, "pwd", 4))
		return (1);
	else if (!ft_strncmp(cmd, "export", 7))
		return (1);
	else if (!ft_strncmp(cmd, "unset", 6))
		return (1);
	else if (!ft_strncmp(cmd, "env", 4))
		return (1);
	else if (!ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
}

t_list	**alphasort_env(t_list **alpha_env)
{
	t_list	*ptr;
	t_list	*ptr_bis;
	void	*temp;
	char	*str_next;
	char	*str_content;

	ptr = *alpha_env;
	while (ptr)
	{
		ptr_bis = *alpha_env;
		while (ptr_bis && ptr_bis->next)
		{
			str_next = (char *)ptr_bis->next->content;
			str_content = (char *)ptr_bis->content;
			if (str_content[0] > str_next[0])
			{
				temp = ptr_bis->next->content;
				ptr_bis->next->content = ptr_bis->content;
				ptr_bis->content = temp;
			}
			ptr_bis = ptr_bis->next;
		}
		ptr = ptr->next;
	}
	return (alpha_env);
}

t_list	**get_alpha_env(t_list **env)
{
	t_list	**alpha_env;

	alpha_env = fill_dishwasher(ft_lstdup(env, NULL), free, get_sink(NULL));
	if (!alpha_env)
		return (NULL);
	alpha_env = alphasort_env(alpha_env);
	return (alpha_env);
}

void	*find_smallest_var(t_list **env)
{
	t_list	*ptr;
	void	*smallest;
	char	*str_smallest;
	char	*str_content;

	ptr = *env;
	smallest = (*env)->content;
	while (ptr)
	{
		str_smallest = (char *)smallest;
		str_content = (char *)ptr->content;
		if (str_smallest[0] > str_content[0])
			smallest = ptr->content;
		ptr = ptr->next;
	}
	return (smallest);
}

void	print_quoted_env(char *var_env)
{
	int	i;

	i = 0;
	while (var_env && var_env[i] && var_env[i] != '=')
		write(1, &var_env[i++], 1);
	if (var_env && var_env[i] && var_env[i] == '=')
	{
		write(1, &var_env[i++], 1);
		write(1, "\"", 1);
		while (var_env && var_env[i])
			write(1, &var_env[i++], 1);
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
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
