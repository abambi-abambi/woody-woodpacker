/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_read_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 10:33:53 by abambi            #+#    #+#             */
/*   Updated: 2022/09/10 15:32:56 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	manage_file(t_woody *woody, char *filename)
{
	int		fd;

	fd = open_file(woody, filename);
	if (fd == -1)
		return (-1);
	else
		woody->filesize = get_filesize(woody, fd);
	if (woody->filesize < (long)(sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + sizeof(Elf64_Shdr)))
		write(2, E_FILE_INVALID, 47);
	else
	{
		woody->addr = malloc(woody->filesize);
		if (woody->addr == NULL)
			write(2, E_MALLOC, 29);
		if (woody->addr != NULL)
			ft_memset(woody->addr, '\0', woody->filesize);
		if (woody->addr != NULL)
			woody->res = copy_info(woody, fd);
	}
	close(fd);
	if (woody->res == 1)
		woody->res = detect_elftype(woody);
	if (woody->res == 1)
		woody->res = prepare_to_write(woody);
	if (woody->res == 1)
		woody->res = write_to_file(woody);
	if (woody->res == 1)
		write(woody->logs_fd, "OK\n", 3);
	close(woody->logs_fd);
	if (woody->i_flag == 'i')
		print_logs();
	free(woody->addr);
	return (0);
}

int	open_file(t_woody *woody, char *filename)
{
	int		fd;
	char	s[100];
	
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		write(2, E_OPEN, 33);
		return (-1);
	}
	if (read(fd, NULL, 0) < 0)
	{
		close(fd);
		write(2, E_READ, 33);
		return (-1);
	}
	sprintf(s, "- opened file with new fd:\t\t[%d], OK\n", fd);
	write(woody->logs_fd, s, ft_strlen(s));// if (fd == -1) EBADF //https://ru.manpages.org/write/2 // https://linux.die.net/man/2/write
	return (fd);
}

ssize_t	get_filesize(t_woody *woody, int fd)
{
	ssize_t	end_position;
	char	s[100];

	end_position = lseek(fd, (size_t)0, SEEK_END);
	if (end_position < 0 || lseek(fd, (size_t)0, SEEK_SET) == -1)
	{
		close(fd);
		write(2, E_LSEEK, 46);
		return (-1);
	}
	sprintf(s, "- gotten filesize:\t\t\t\t[%li], OK\n", end_position);
	write(woody->logs_fd, s, ft_strlen(s));
	return (end_position);
}

int	copy_info(t_woody *woody, int fd)
{
	if (read(fd, woody->addr, woody->filesize) != woody->filesize)
	{
		close(fd);
		free(woody->addr);
		write(2, E_COPY, 43);
		return (0);
	}
	write(woody->logs_fd, "- copyed data from the old fd to the new *ptr... ", 49);
	return (1);
}

int	detect_elftype(t_woody *woody)
{
	unsigned char *c;

	c = woody->addr;
	write(woody->logs_fd, "OK\n- detecting elftype...", 25);
	if (\
		c[0] == 0x7f && \
		c[1] == 'E' && \
		c[2] == 'L' && \
		c[3] == 'F' && \
		c[4] == ELFCLASS64 && \
		(c[16] == ET_EXEC || c[16] == ET_DYN) && \
		c[18] == EM_X86_64)
		return (1);
	else
	{
		write(2, E_FILE_INVALID, 47);
		return (0);
	}
}
