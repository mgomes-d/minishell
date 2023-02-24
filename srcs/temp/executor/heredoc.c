/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 12:17:41 by fluchten          #+#    #+#             */
/*   Updated: 2023/02/23 15:35:38 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_heredoc(t_lexer *heredoc, bool quotes, t_data *data, char *file_name)
{
	int		fd;
	char	*line;

	fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	line = readline("\033[1;34m> \033[0m");
	while (line && ft_strncmp(heredoc->str, line, ft_strlen(heredoc->str))
		&& !g_global.stop_heredoc)
	{
		if (quotes == false)
			line = expander_str(data, line);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("\033[1;34m> \033[0m");
	}
	free(line);
	if (g_global.stop_heredoc || !line)
		return (EXIT_FAILURE);
	close(fd);
	return (EXIT_SUCCESS);
}

static int	ft_heredoc(t_data *data, t_lexer *heredoc, char *file_name)
{
	bool	quotes;
	int		sl;

	sl = EXIT_SUCCESS;
	if ((heredoc->str[0] == '\"'
			&& heredoc->str[ft_strlen(heredoc->str) - 1] == '\"')
		|| (heredoc->str[0] == '\''
			&& heredoc->str[ft_strlen(heredoc->str) - 1] == '\''))
		quotes = true;
	else
		quotes = false;
	delete_quotes(heredoc->str, '\"');
	delete_quotes(heredoc->str, '\'');
	g_global.stop_heredoc = 0;
	g_global.in_heredoc = 1;
	sl = create_heredoc(heredoc, quotes, data, file_name);
	g_global.in_heredoc = 0;
	data->heredoc = true;
	return (sl);
}

static char	*generate_heredoc_filename(void)
{
	static int	i = 0;
	char		*num;
	char		*file_name;

	num = ft_itoa(i++);
	file_name = ft_strjoin("build/.tmp_heredoc_file_", num);
	free(num);
	return (file_name);
}

int	send_heredoc(t_data *data, t_cmds *cmd)
{
	t_lexer	*start;
	int		sl;

	start = cmd->redirections;
	sl = EXIT_SUCCESS;
	while (cmd->redirections)
	{	
		if (cmd->redirections->token == LESS_LESS)
		{
			if (cmd->hd_file_name)
				free(cmd->hd_file_name);
			cmd->hd_file_name = generate_heredoc_filename();
			sl = ft_heredoc(data, cmd->redirections, cmd->hd_file_name);
			if (sl)
			{
				g_global.error_num = 1;
				return (reset_data(data));
			}
		}
		cmd->redirections = cmd->redirections->next;
	}
	cmd->redirections = start;
	return (EXIT_SUCCESS);
}
