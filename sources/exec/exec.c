/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 17:10:10 by gkehren           #+#    #+#             */
/*   Updated: 2022/10/17 14:45:42 by gkehren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_error(t_list *lcmd, t_list *venv)
{
	perror("Minishell");
	ft_lstclear(&lcmd, &del_cmd);
	ft_lstclear(&venv, &del_venv);
	exit(1);
}

int	parent_process(t_list *lcmd, char **env, int *fdd)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		fd[2];

	cmd = (t_cmd *)lcmd->content;
	pid = fork();
	if (pipe(fd) == -1 || pid == -1)
		return (free_double_tab((void *)env), 1);
	else if (pid == 0)
	{
		if (get_token_id(cmd->token_files) == IN)
			child_process_in(fd, fdd, env, lcmd);
		else if (get_token_id(cmd->token_files) == OUT)
			child_process_out(fd, fdd, env, lcmd);
		else
			child_process(fd, fdd, env, lcmd);
	}
	else
	{
		free_double_tab((void *)env);
		close(fd[1]);
		(*fdd) = fd[0];
		wait(NULL);
	}
	return (0);
}

int	exec(t_list *lcmd, t_list *venv)
{
	t_cmd	*cmd;
	int		fdd;

	fdd = 0;
	cmd = (t_cmd *)lcmd->content;
	while (lcmd)
	{
		if (cmd->builtin != NULL)
			only_builtins(cmd->builtin, cmd, venv);
		else if (parent_process(lcmd, send_env(&venv), &fdd) == 1)
			return (exec_error(lcmd, venv), 1);
		lcmd = lcmd->next;
	}
	return (0);
}
