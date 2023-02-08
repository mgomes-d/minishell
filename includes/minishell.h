/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 14:59:18 by fluchten          #+#    #+#             */
/*   Updated: 2023/02/08 11:25:36 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include "libft.h"
# include "get_next_line.h"

typedef struct s_data
{
	char	*line;
}			t_data;

typedef enum s_tokens
{
	R, // > = 0
	L, // < = 1
	RR, // >> = 2
	LL, // < = 3
	P // | = 4
}			t_tokens;

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	int				i; // str = 0, token = 1;
	struct s_lexer	*next;
}					t_lexer;

/* parsing */
t_lexer	*parser(char **line);
void	print_list(t_lexer *list);
/* utils */
int		print_error(char *str);

#endif