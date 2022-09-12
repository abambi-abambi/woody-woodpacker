/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_write_to_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 10:33:53 by abambi            #+#    #+#             */
/*   Updated: 2022/09/08 14:55:35 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int write_to_file(t_woody *woody)
{
	int		fd;
	ssize_t	i;
	void	*ptr;

	ptr = woody->addr;
	fd = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (fd > 0)
	{
		i = 0;
		while (i <  woody->filesize)// instead of i = write(fd, woody->addr, woody->filesize);
		{
			ptr = woody->addr + i;
			if ( woody->filesize - i >= CHUNK)
				i += write(fd, ptr, CHUNK);
			else
				i += write(fd, ptr,  woody->filesize % CHUNK);
		}
		close(fd);
		write(woody->logs_fd, "- written information to 'woody' file... \n", 42);
	}
	else
	{
		write(2, E_WRITE_WOODY, 39);
		return (-1);
	}
	return (i == woody->filesize);
}

int	print_logs(void)
{
	ssize_t filesize;
	char	buf[1001];
	int		len;
	int		new_fd;

	len = -1;
	new_fd = open("log.txt", O_RDONLY);
	if (new_fd != -1)
	{
		filesize = lseek(new_fd, (size_t)0, SEEK_END);
		if (filesize < 0 || lseek(new_fd, (size_t)0, SEEK_SET) == -1)
		{
			close(new_fd);
			write(2, "Error of finding size of 'log.txt' for printing logs", 52);
			return (-1);
		}
		printf("\n[new fd of log.txt: %i\n", new_fd);
		printf("size of log.txt: %li]\n\n", filesize);
		printf("Start printing logs:\n");
		for (int i = 0; i < filesize; )
		{
			len = read(new_fd, buf, 200);
			if (len < 0)
				break ;
			buf[len] = '\0';
			write(1, buf, len);
			i += len;
		}
	}
	if (new_fd == -1 || len == -1)
		write(2, "Error of reading 'log.txt' for printing logs\n", 45);
	close(new_fd);
	return (0);
}
