/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hd_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:02:19 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/11 15:14:22 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_hd_utils(t_hd_utils *hd_utils, char *f, char *str, size_t *i)
{
	hd_utils->formatted = f;
	hd_utils->str = str;
	hd_utils->i = i;
}

void	init_hd_utils_2(t_hd_utils *hd, size_t *b, char **cmd, t_list **env)
{
	hd->begin_part = b;
	hd->cmd = cmd;
	hd->env = env;
}

void	init_hd_utils_3(t_hd_utils *hd, char *f, char **envp, t_sink **bin)
{
	hd->formatted = f;
	hd->envp = envp;
	hd->bin = bin;
}
