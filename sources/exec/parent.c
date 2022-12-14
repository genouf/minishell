/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:25:05 by gkehren           #+#    #+#             */
/*   Updated: 2022/10/27 12:25:06 by gkehren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_parent(t_list *lcmd, pid_t *pid, t_cmd **cmd, int (*fd)[])
{
	*cmd = (t_cmd *)lcmd->content;
	if (pipe(*fd) == -1)
		return (1);
	*pid = fork();
	if (*pid == -1)
		return (1);
	return (0);
}

static void	core_parent_process(int *fdd, int (*fd)[], t_list *lcmd, t_cmd *cmd)
{
	close((*fd)[1]);
	if (*fdd != -2)
		close(*fdd);
	(*fdd) = (*fd)[0];
	if (lcmd->next == NULL)
		close(*fdd);
	if (cmd->files->infile != -2)
		close(cmd->files->infile);
	if (cmd->files->outfile != -2)
		close(cmd->files->outfile);
}

int	parent_process(t_exec *exec, t_list *lcmd, int *fdd)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		fd[2];

	signal(SIGINT, handle_sigint_pro);
	signal(SIGQUIT, handle_sigquit_pro);
	if (init_parent(lcmd, &pid, &cmd, &fd))
		return (1);
	if (pid == 0)
	{
		if (cmd->builtin)
			child_process_builtins(fd, fdd, exec, lcmd);
		else
			child_process(fd, fdd, exec, lcmd);
	}
	else
		core_parent_process(fdd, &fd, lcmd, cmd);
	return (0);
}
