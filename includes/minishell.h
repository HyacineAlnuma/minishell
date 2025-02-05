/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: secros <secros@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:02:33 by secros            #+#    #+#             */
/*   Updated: 2025/02/05 14:46:14 by secros           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define CHARSET " \t\n\v\f\r"

typedef struct s_com
{
	char	*command;
	char	**option;
	char	*infile;
	char	*outfile;
}	t_com;

#endif