/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:04:53 by secros            #+#    #+#             */
/*   Updated: 2025/04/18 15:20:14 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	verif_doc(t_doc **docs)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	if (!docs)
		return ;
	while (docs[i])
	{
		if (docs[i]->type >= HEREDOC)
			count++;
		i++;
	}
	i = 0;
	while (docs[i] && count > 1)
	{
		if (docs[i]->type >= HEREDOC)
		{
			ft_printf("YOOOO\n");
			docs[i]->str = NULL;
			close(docs[i]->type);
			count--;
		}
		i++;
	}
}

t_exec	*setup_exec(t_list **piped, t_sink **bin, t_list **env)
{
	t_exec	*new;
	char	**tab;

	if (!piped || !*piped)
		return (NULL);
	new = new_plate(sizeof(t_exec), bin);
	if (!new)
		return (NULL);
	ft_bzero(new, sizeof(t_exec));
	new->docs = create_docs(piped, *piped, *bin, env);
	verif_doc(new->docs);
	if (!new->docs || g_sigint_flag == 2)
		return (NULL);
	new->bin = bin;
	merge_all(*piped, *bin);
	ft_lst_hand_wash_if(piped, NULL, compare, *bin);
	tab = convert_lst_in_tab(*piped, *bin);
	if (!tab)
		return (NULL);
	new->cmd = tab[0];
	new->opt = tab;
	return (new);
}

t_list	**str_to_tokens(char *str, t_list **env, t_sink **bin, size_t *count)
{
	t_list	**piped;
	t_list	*tokens;

	if (!str)
		return (NULL);
	tokens = create_token_list(str, bin);
	if (!tokens)
		return (NULL);
	if (!env_handling(tokens, env, *bin))
		return (NULL);
	*count = lst_count_char(tokens, '|');
	piped = fill_dishwasher(cut_instruction(tokens, *count), free, bin);
	return (piped);
}

t_exec	**parsing(char *str, t_list **env, t_sink **bin)
{
	t_list	**piped;
	t_exec	**exec;
	size_t	count;
	size_t	i;

	piped = str_to_tokens(str, env, bin, &count);
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
