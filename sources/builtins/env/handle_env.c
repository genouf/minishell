#include "minishell.h"

static int	*count_size_env(char *str, char charset)
{
	int	i;
	int	count;
	int	*result;

	result = (int *)malloc(sizeof(int) * 2);
	if (result == NULL)
		return (NULL);
	i = -1;
	count = 0;
	while (str[++i] && str[i] != charset && str[i] != '+')
		count++;
	if (str[i] == '+')
		i++;
	result[0] = count + 1;
	count = 0;
	while (str[i] && str[++i])
		count++;
	result[1] = count + 1;
	return (result);
}

static void	set_env_str(char **result, int *size_strs, char *env_part, int num)
{
	int	i;

	i = 0;
	while (i < size_strs[num] - 1)
	{
		result[num][i] = *env_part;
		env_part++;
		i++;
	}
	result[num][i] = '\0';
}

static int	handle_malloc_env(char ***result, int **size_strs, char **env_part)
{
	*size_strs = count_size_env(*env_part, '=');
	if (*size_strs == NULL)
		return (1);
	*result = (char **)malloc(sizeof(char *) * 2);
	if (*result == NULL)
	{
		ft_free((void **)*size_strs);
		return (1);
	}
	(*result)[0] = (char *)malloc(sizeof(char) * (*size_strs)[0]);
	if ((*result)[0] == NULL)
	{
		ft_free((void **)*size_strs);
		ft_free((void **)(*result));
		return (1);
	}
	(*result)[1] = (char *)malloc(sizeof(char) * (*size_strs)[1]);
	if ((*result)[1] == NULL)
	{
		ft_free((void **)*size_strs);
		ft_free((void **)(*result)[0]);
		ft_free((void **)*result);
		return (1);
	}
	return (0);
}

char	**cut_env(char *env_part)
{
	char	**result;
	int		*size_strs;

	if (find_char('=', env_part))
		result = malloc_not_init(env_part);
	else
	{
		if (handle_malloc_env(&result, &size_strs, &env_part))
			return (NULL);
		set_env_str(result, size_strs, env_part, 0);
		set_env_str(result, size_strs, env_part + size_strs[0], 1);
		ft_free((void **)&size_strs);
	}
	return (result);
}

int	parse_env_data(t_list **venv, char **env)
{
	char	**tmp_char;
	t_venv	*tmp_venv;
	int		i;

	i = 0;
	while (env[i])
	{
		tmp_char = cut_env(env[i]);
		if (tmp_char == NULL)
			return(malloc_failed_env(venv));
		tmp_venv = init_venv(tmp_char[0], tmp_char[1], INIT);
		if (tmp_venv == NULL)
			return(malloc_failed_env(venv));
		ft_lstadd_back(venv, ft_lstnew((void *)tmp_venv));
		ft_free((void **)&tmp_char);
		i++;
	}
	return (0);
}