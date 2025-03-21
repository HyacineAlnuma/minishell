/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/03/21 14:43:46 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lst_len(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

void	print_lsts(t_list **lst)
{
	int	j;

	j = 0;
	while (lst[j])
	{
		ft_printf("[%s]", (char *)lst[j]->content);
		lst[j] = lst[j]->next;
		if (!lst[j])
		{
			ft_printf("\n");
			j++;
		}
	}
}

t_exec	**parsing(char *str, t_list **env, t_sink *bin)
{

	t_list	**piped;
	t_list	*tokens;
	t_list	*tmp;
	t_exec	**exec;
	int		count;
	int		i;
	int		j;
	char	**tab;

	if (!str)
		return (NULL);
	tokens = create_token_list(str, bin);
	if (!tokens)
		return (NULL);
	env_handling(tokens, env, bin);
	count = lst_count_char(tokens, '|');
	piped = fill_dishwasher(cut_instruction(tokens, count), free, &bin);
	exec = new_plate((sizeof(t_exec *) * (count + 2)), &bin);
	ft_bzero(exec, (sizeof(t_exec *) * (count + 2)));
	i = 0;
	while (i <= count)
	{
		exec[i] = fill_dishwasher(ft_calloc(sizeof(t_exec), 1), free, &bin);
		exec[i]->docs = create_docs(piped[i], bin);
		exec[i]->bin = bin;
		merge_all(piped[i], bin);
		ft_lst_hand_wash_if(&piped[i], NULL, compare, bin);
		tab = fill_dishwasher(ft_calloc(sizeof(char *), lst_len(piped[i]) + 1), free, &bin);
		tmp = piped[i];
		j = 0;
		while (tmp)
		{
			tab[j++] = tmp->content;
			tmp = tmp->next;
		}
		exec[i]->cmd = tab[0];
		exec[i]->opt = tab;
		i++;
	}
	return (exec);
}
