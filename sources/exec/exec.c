/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: genouf <genouf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 17:10:10 by gkehren           #+#    #+#             */
/*   Updated: 2022/10/20 18:41:06 by genouf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exec(t_list *lcmd, t_list *venv)
{
	ft_lstclear(&lcmd, &del_cmd);
	ft_lstclear(&venv, &del_venv);
	exit(1);
}

static void	init_exec(t_exec *exec, t_list **lcmd, t_list **venv)
{
	exec->cmd = lcmd;
	exec->venv = venv;
}

int	exec(t_list **lcmd, t_list **venv)
{
	t_cmd	*cmd;
	t_exec	exec;
	int		fdd;
	int		size_cmd;
	t_list	*tmp_list;

	tmp_list = *lcmd;
	fdd = -2;
	init_exec(&exec, lcmd, venv);
	size_cmd = ft_lstsize(*lcmd);
	while (tmp_list)
	{
		cmd = (t_cmd *)(tmp_list)->content;
		if (cmd->builtin != NULL && size_cmd == 1)
			only_builtins(cmd->builtin, cmd, *venv);
		else if (parent_process(&exec, tmp_list, &fdd) == 1)
			return (free_exec(tmp_list, *venv), 1);
		if (cmd->files->is_heredoc == 1)
			unlink(cmd->files->index_cmd_str);
		tmp_list = (tmp_list)->next;
	}
	while (size_cmd)
	{
		wait(NULL);
		size_cmd--;
	}
	begin_signal();
	return (0);
}
