/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lib.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:20:21 by abambi            #+#    #+#             */
/*   Updated: 2022/09/10 10:00:26 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	ft_memcmp(const void *memptr1, const void *memptr2, size_t num)
{
	unsigned char *ptr1;
	unsigned char *ptr2;

	ptr1 = (unsigned char *)memptr1;
	ptr2 = (unsigned char *)memptr2;
	if (memptr1 == memptr2 || num == 0)
		return (0);
	while (num-- != 0)
	{
		if (*ptr1 != *ptr2)
			return (*ptr1 - *ptr2);
		ptr1++;
		ptr2++;
	}
	return (0);
}

void	*ft_memmove(void *to, const void *from, size_t count)
{
	unsigned char *t;
	unsigned char *f;

	if (to == from)
		return (to);
	t = (unsigned char *)to;
	f = (unsigned char *)from;
	if (t < f)
		while (count--)
			*t++ = *f++;
	else
	{
		t += count;
		f += count;
		while (count--)
			*--t = *--f;
	}
	return (to);
}

void	*ft_memset(void *dest, int c, size_t len)
{
	unsigned char ch;
	unsigned char *ptr;

	ch = (unsigned char)c;
	ptr = dest;
	while (len-- > 0)
	{
		*ptr = ch;
		ptr++;
	}
	return (dest);
}

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int	ft_strncmp(const char *string1, const char *string2, size_t num)
{
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	if (num == 0)
		return (0);
	ptr1 = (unsigned char *)string1;
	ptr2 = (unsigned char *)string2;
	while ((*ptr1 != '\0' || *ptr2 != '\0') && num--)
	{
		if (*ptr1 != *ptr2)
			return (*ptr1 - *ptr2);
		ptr1++;
		ptr2++;
	}
	return (0);
}
