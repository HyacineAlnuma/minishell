/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:59:00 by secros            #+#    #+#             */
/*   Updated: 2025/04/11 11:45:48 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quote(char *str, t_sink *bin)
{
	char	*tmp;

	if (!str)
		return (NULL);
	tmp = str;
	if (str[0] == '\'' || str[0] == '"')
	{
		tmp = fill_dishwasher(ft_substr(str, 1, ft_strlen(str) - 2), free, &bin);
		if (tmp[0] == '\0')
		{
			hand_wash(tmp, &bin);
			return (NULL);
		}
	}
	return (tmp);
}

int	env_handling(t_list *tokens, t_list **env, t_sink *bin)
{
	char	*token;
	char	*new_str;

	while (tokens)
	{
		token = (char *)tokens->content;
		if (token && token[0] != '\'')
		{
			new_str = handle_env(token, env, bin);
			if (!new_str)
				return (0);
			tokens->content = new_str;
		}
		tokens = tokens->next;
	}
	return (1);
}

int	compare(char *str, char *str_ref)
{
	if (!str || !str_ref)
	{
		if (!str && !str_ref)
			return (0);
		else
			return (1);
	}
	return (ft_strcmp(str, str_ref));
}

void	merge_all(t_list *lst, t_sink *bin)
{
	while (lst)
	{
		merge_tokens(lst, bin);
		lst = lst->next;
	}
}

void	ft_lst_hand_wash_if(t_list **begin_list, void *data_ref, \
int (*cmp)(), t_sink *bin)
{
	t_list	*tmp;

	if ((*begin_list == NULL) || (begin_list == NULL))
		return ;
	if (cmp((*begin_list)->content, data_ref) == 0)
	{
		tmp = *begin_list;
		*begin_list = (*begin_list)->next;
		hand_wash(tmp, &bin);
		ft_lst_hand_wash_if(begin_list, data_ref, cmp, bin);
	}
	else
		ft_lst_hand_wash_if(&((*begin_list)->next), data_ref, cmp, bin);
}
