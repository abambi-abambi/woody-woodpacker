/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_encrypt_code.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 10:33:53 by abambi            #+#    #+#             */
/*   Updated: 2022/09/10 11:40:36 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	encrypt_target(t_woody *woody)
{
	t_dset		dset;
	uint64_t	key;
	void		*data;

	data = woody->addr + woody->text->sh_offset;
	key = create_random_key();
	encrypt_text_section(woody, data, key);
	init_dset_struct(&dset, woody, key);
	change_load_segment(&dset, woody);
	return (0);
}

/*
** Key should be 16 characters long (0-9, A-F)
**
** '/dev/random' and '/dev/urandom' — are special symbolic pseudo-devices in some UNIX-like systems 
** that first appeared in the Linux kernel version 1.3.30
**
** gives random 64-bit key from '/dev/urandom'
*/

uint64_t	create_random_key(void)
{
	int			fd;
	uint64_t	buf;
	int			len;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		buf = another_keygenerator();
	}
	len = read(fd, &buf, 8);
	if (len < 8)
	{
		buf = another_keygenerator();
	}
	close(fd);
	return (buf);
}

/*
** The standard implementation of the rand function in "old C"
** https://habr.com/ru/post/499490/
*/

static unsigned long int next_rand = 1;

void	ft_srand(unsigned int seed)
{
	next_rand = seed;
}

int	ft_rand(void)
{
	next_rand = next_rand * 1103515245 + 12345;
	return (unsigned int)(next_rand / 65536) % 32768;
}

/*
** END of the standard implementation of the rand function in "old C"
*/

int	another_keygenerator()
{
	struct timeval	t_actual;
	char			kkey[16];
	size_t			i;

	gettimeofday(&t_actual, NULL);
	ft_srand(t_actual.tv_sec);
	for (i = 0; i < 16; i++)
		kkey[i] = ft_rand() % 16;	
	return (atoi(kkey));
}

/*
** Explanation of rc4 encryption (with images): 
** https://habr.com/ru/post/111510/
*/

/*
** Sample of easier (rc4) encryption:
**
** 	for (i = 0; i < input_len; i++)
** 	{
** 		encrypt[i] = input[i] ^ key->str[j];
** 		j++;
** 		if (j == key->size)
** 			j = 0;
** 	}
** 	return (encrypt);
*/

/*
** https://learnc.info/c/bitwise_operators.html
**
** for each byte of data:
** - simple 'xor' with lower 8 bits of key, 
** - rotate key to the right '>>' by 1
*/

uint64_t	encrypt_text_section(t_woody *woody, void *data, uint64_t key)
{
	uint64_t	key_copy;// unsigned long long // 8 bite (64 bits)
	char		s[200];
	uint64_t	value;

	key_copy = key;
	write(woody->logs_fd, "- start encrypting code... ", 27);
	for (size_t i = 0; i < woody->text->sh_size; i++)
	{
		*(unsigned char *)(data + i) = *(unsigned char *)(data + i) ^ (key & 0b11111111);// get last 8 bits of the value // The least significant byte is the 8-bits at the right-hand side.
		value = key & 0b0000001;// 0 or 1
		value = value << 63;	// 0 or -9223372036854775808
		key = (key / 2) | value;// key = (key >> 1) | value;
	}
	printf(KEY_FORMAT, (long long)key_copy);
	printf("File create: woody\n");

	sprintf(s, "success\n- set of encrypted code,\n\t\tsize (of text section):\t[%li], address: [%p], OK\n", woody->text->sh_size, data);
	write(woody->logs_fd, s, ft_strlen(s));

	return (key_copy);
}

/* 
** Update the e_entry in elf64_header
** to point to decryptor part of start.
**
** It must jump back to original entry point after decryption.
*/

uint64_t	set_new_entry(t_woody *woody)
{
	Elf64_Ehdr	*ehdr;

	ehdr = (Elf64_Ehdr	*)woody->addr;
	ehdr->e_entry = woody->code->p_vaddr + woody->code->p_memsz;

	return (ehdr->e_entry);
}

int	init_dset_struct(t_dset *dset, t_woody *woody, uint64_t key)
{
	dset->original_entry = woody->ehdr->e_entry;
	dset->encrypted_code = woody->text->sh_addr;
	dset->encrypted_size = woody->text->sh_size;
	dset->key = key;
	dset->encrypted_entry = set_new_entry(woody);// check: readelf -l woody and readelf -l original_program

	return (0);
}

int	change_load_segment(t_dset *dset, t_woody *woody)
{
	void		*ptr;
	ssize_t		len;
	char		s[400];

	ptr = woody->addr + woody->code->p_offset + woody->code->p_filesz;
	woody->code->p_memsz += g_decryptor_len;
	woody->code->p_filesz += g_decryptor_len;
	woody->code->p_flags = woody->code->p_flags | PF_W;// add the writable flag: PF_W (== 0b010); all flags: PF_R | PF_W | PF_X == 0b111 == 0x7
	len = g_decryptor_len - sizeof(t_dset);
	ft_memmove(ptr, g_decryptor, len);
	ft_memmove(ptr + len, dset, sizeof(t_dset));

	sprintf(s, "- change load segment:\n\tsize of descriptor part:\t[%u],\taddress: [%p],\
\n\t\t\t\told p_memsz:\t[%li],\tnew p_memsz: [%li],\
\n\t\t\t\told p_filesz:\t[%li],\tnew p_filesz: [%li],\
 OK\n", g_decryptor_len, ptr, \
(woody->code->p_memsz - g_decryptor_len), woody->code->p_memsz,\
(woody->code->p_filesz - g_decryptor_len), woody->code->p_filesz);
	write(woody->logs_fd, s, ft_strlen(s));

	return (0);
}
