/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/04/11 16:49:43 by halnuma          ###   ########.fr       */
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

char	**convert_lst_in_tab(t_list *lst, t_sink *bin)
{
	char	**tab;
	size_t	nb_tok;

	nb_tok = lst_len(lst);
	tab = new_plate(sizeof(char *) * (nb_tok + 1), &bin);
	if (!tab)
		return (NULL);
	tab[nb_tok] = NULL;
	nb_tok = 0;
	while (lst)
	{
		tab[nb_tok++] = lst->content;
		lst = lst->next;
	}
	return (tab);
}

t_exec	*setup_exec(t_list **piped, t_sink **bin, t_list **env)
{
	t_exec	*new;
	char	**tab;

	new = new_plate(sizeof(t_exec), bin);
	if (!new)
		return (NULL);
	ft_bzero(new, sizeof(t_exec));
	new->docs = create_docs(piped, *piped, *bin, env);
	if (!new->docs)
		return (NULL);
	new->bin = bin;
	merge_all(*piped, *bin);
	ft_lst_hand_wash_if(piped, NULL, compare, *bin);
	// if (new->docs && ! piped)
	tab = convert_lst_in_tab(*piped, *bin);
	if (!tab)
		return (NULL);
	new->cmd = tab[0];
	new->opt = tab;
	return (new);
}

void	print_lst(t_list *lst)
{
	while (lst)
	{
		ft_printf("--%s--\n", lst->content);
		lst = lst->next;
	}
	ft_printf("\n");

}

t_exec	**parsing(char *str, t_list **env, t_sink **bin)
{
	t_list	**piped;
	t_list	*tokens;
	t_exec	**exec;
	size_t	count;
	size_t	i;

	if (!str)
		return (NULL);
	tokens = create_token_list(str, *bin);
	if (!tokens)
		return (NULL);
	if (!env_handling(tokens, env, *bin))
		return (NULL);
	count = lst_count_char(tokens, '|');
	piped = fill_dishwasher(cut_instruction(tokens, count), free, bin);
	if (!piped)
		return (NULL);
	exec = new_plate((sizeof(t_exec *) * (count + 2)), bin);
	if (!exec)
		return (NULL);
	ft_bzero(exec, (sizeof(t_exec *) * (count + 2)));
	i = 0;
	while (piped[i])
	{
		exec[i] = setup_exec(&piped[i], bin, env);
		if (!exec[i])
			return (NULL);
		i++;
	}
	return (exec);
}
