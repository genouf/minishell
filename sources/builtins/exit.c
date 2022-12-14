/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:28:38 by gkehren           #+#    #+#             */
/*   Updated: 2022/10/27 12:28:39 by gkehren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_status(long long int_status, t_list **venv, t_exec *exec)
{
	int_status = int_status % 256;
	printf("exit\n");
	ft_lstclear(venv, &del_venv);
	ft_lstclear(exec->cmd, &del_cmd);
	exit(int_status);
}

static int	checker_integer(char *str, char *tmp, char *tmp2)
{
	if (str[0] == '-')
	{
		if (ft_strncmp(str, tmp, 1) != 0)
		{
			free(tmp);
			return (0);
		}
		str++;
		tmp2 = tmp;
		tmp = ft_strdup(tmp + 1);
		free(tmp2);
	}
	else if (str[0] == '+')
		str++;
	while (*str == '0' && str[1] != '\0')
		str++;
	if (ft_strcmp(str, tmp) == 0)
	{
		free(tmp);
		return (0);
	}
	free(tmp);
	return (1);
}

static int	check_parsing(char **args)
{
	int	i;

	i = 0;
	while (args[1][i] >= 9 && args[1][i] <= 12)
		i++;
	if (args[1][i] == '+' || args[1][i] == '-')
		i++;
	while (ft_isdigit(args[1][i]))
		i++;
	if (args[1][i] != '\0')
		return (1);
	return (0);
}

static int	check_errors(char **args)
{
	char		*tmp;

	if (check_parsing(args))
		return (print_error_str("exit\nminishell: exit: ", args[1],
				": numeric argument required\n"), 2);
	if (ft_atoi(args[1]) != 0 && ft_atoll(args[1]) == 0)
		return (print_error_str("exit\nminishell: exit: ", args[1],
				": numeric argument required\n"), 2);
	if (ft_atoi(args[1]) == LLONG_MIN && ft_atoll(args[1]) == LLONG_MIN)
		return (0);
	tmp = ft_itoa(ft_atoi(args[1]));
	if (tmp == NULL)
		return (1);
	if (checker_integer(args[1], tmp, NULL))
		return (print_error_str("exit\nminishell: exit: ", args[1],
				": numeric argument required\n"), 2);
	return (0);
}

int	ft_exit(int argc, char **args, t_list **venv, t_exec *exec)
{
	int	ret;

	(void)venv;
	if (argc == 1)
		exit_status(g_global.g_status, venv, exec);
	ret = check_errors(args);
	if (ret == 2)
		exit_status(2, venv, exec);
	else if (ret == 1)
	{
		ft_putstr_fd("Malloc failed !", 2);
		exit_status(1, venv, exec);
	}
	if (argc > 2)
		return (set_status(1),
			print_error("exit\nminishell: exit: too many arguments\n"));
	exit_status(ft_atoi(args[1]), venv, exec);
	return (0);
}
