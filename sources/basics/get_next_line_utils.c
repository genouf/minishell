/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:29:42 by gkehren           #+#    #+#             */
/*   Updated: 2022/10/27 12:29:42 by gkehren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_calloc(size_t count, size_t size)
{
	void			*result;
	unsigned char	*p;
	size_t			buff_size;

	buff_size = count * size;
	result = (void *)malloc(buff_size);
	if (result == NULL)
		return (NULL);
	p = (unsigned char *)result;
	while (buff_size > 0)
	{
		*p = 0;
		p++;
		buff_size--;
	}
	return (result);
}

void	ft_substr(char *s, unsigned int start, char *save)
{
	size_t	i;
	size_t	size;

	start++;
	size = ft_strlen(s + start);
	i = 0;
	if (size == 0)
		return ;
	while (s[start] != '\0')
	{
		save[i] = s[start];
		i++;
		start++;
	}
}

char	*ft_subuff(char *buff, int id_line)
{
	char	*result;
	int		i;

	result = (char *)ft_calloc(sizeof(char), (id_line + 2));
	if (result == NULL)
		return (NULL);
	i = 0;
	while (i < id_line + 1)
	{
		result[i] = buff[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
