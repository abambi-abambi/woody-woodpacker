/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_prepare_to_write.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 10:33:53 by abambi            #+#    #+#             */
/*   Updated: 2022/09/09 16:15:52 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	prepare_to_write(t_woody *woody)
{
	woody->ehdr = (Elf64_Ehdr *)woody->addr;
	woody->segments = (Elf64_Phdr *)(woody->addr + woody->ehdr->e_phoff);
	woody->sections = (Elf64_Shdr *)(woody->addr + woody->ehdr->e_shoff);
	woody->code = get_load_segment(woody);
	woody->text = get_text_section(woody);
	if (under_our_encryption(woody) == 1)
	{
		write(2, E_UNDER_ENCRYPTION, 70);
		return (-1);
	}
	if (get_zerofilled_size(woody) < g_decryptor_len)
	{
		write(2, E_NOSPACE, 42);
		return (-1);
	}
	encrypt_target(woody);
	return (1);
}

/*
** Find program header which has executable bit set, which contains e_entry,
** p_filesz should be same as p_memsz
*/

Elf64_Phdr	*get_load_segment(t_woody *woody)
{
	char	s[100];

	if (!woody->ehdr->e_phoff || \
		!woody->ehdr->e_phnum)
		return (NULL);
	for (int i = 0; i < woody->ehdr->e_phnum; i++)
	{
		if (woody->segments[i].p_type == PT_LOAD && \
			(woody->segments[i].p_vaddr <= woody->ehdr->e_entry && \
			woody->segments[i].p_vaddr + woody->segments[i].p_memsz > woody->ehdr->e_entry) && \
			woody->segments[i].p_vaddr == woody->segments[i].p_paddr && \
			woody->segments[i].p_memsz == woody->segments[i].p_filesz)
		{
			sprintf(s, " OK\n- found load_segment, number:\t[%i],\taddress: [%p], OK\n", i, &woody->segments[i]);
			write(woody->logs_fd, s, ft_strlen(s));
			return (&woody->segments[i]);
		}
	}
	return (NULL);
}

/*
** the ".text" section contains executable code
**
** This section is of type SHT_PROGBITS. SHT_PROGBITS == program segment.
** Includes code segment (.text), data segment (.data), debugging information, etc.
**
** The attributes are SHF_ALLOC and SHF_EXECINSTR (SHF_EXECINSTR == executable machine instructions)
*/

Elf64_Shdr	*get_text_section(t_woody *woody)
{
	char	*all_sections_names;
	char	s[100];

	all_sections_names = NULL;
	if (!woody->ehdr->e_shoff || \
		!woody->ehdr->e_shnum || \
		woody->ehdr->e_shstrndx >= woody->ehdr->e_shnum)
		return (NULL);
	all_sections_names = (char *)woody->addr + woody->sections[woody->ehdr->e_shstrndx].sh_offset;
	for (int i = 0; i < woody->ehdr->e_shnum; i++)
	{
		if (ft_strncmp(".text", all_sections_names + woody->sections[i].sh_name, 6) == 0 && \
			(woody->sections[i].sh_type == SHT_PROGBITS) && \
			(woody->sections[i].sh_flags & SHF_EXECINSTR))
		{
			sprintf(s, "- found text_section, number:\t[%i],\taddress: [%p], OK\n", i, &woody->sections[i]);
			write(woody->logs_fd, s, ft_strlen(s));
			return (&woody->sections[i]);
		}
	}
	return (NULL);
}

/*
** Compare bytes at entry point
*/

int	under_our_encryption(t_woody *woody)
{
	void		*cryptopoint_start;
	int			res;

	cryptopoint_start = woody->addr + (woody->code->p_offset + woody->code->p_filesz) - g_decryptor_len;
	if (ft_memcmp(cryptopoint_start, g_decryptor, (size_t)(g_decryptor_len - sizeof(t_dset))) == 0)
		res = 1;
	else
		res = 0;
	return (res);
}

size_t		get_zerofilled_size(t_woody *woody)
{
	void	*start;
	void	*pos;
	char	s[100];

	start = woody->addr + woody->code->p_offset + woody->code->p_filesz;
	pos = start;
	while (pos < (woody->addr + woody->filesize) && *(unsigned char *)pos == 0)
	{
		++pos;
	}
	sprintf(s, "- found emptyspace size:\t\t[%li],\taddress: [%p], OK\n", pos - start, pos);
	write(woody->logs_fd, s, ft_strlen(s));
	return (pos - start);
}
