/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 10:33:53 by abambi            #+#    #+#             */
/*   Updated: 2022/09/10 13:11:39 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/time.h>

# define E_FLAG				"Error: wrong flag! Wrong flag is ignored\n"
# define E_FILE_INVALID		"Error: invalid file, must be valid linux elf64\n"
# define E_NOSPACE			"Error: could not find space for decryptor\n"
# define E_COPY				"Error: failed to copy info from input file\n"
# define E_LSEEK			"Error: failed to lseek the size of input file\n"
# define E_MALLOC			"Error: failed to do malloc()\n"
# define E_OPEN				"Error: failed to open input file\n"
# define E_READ				"Error: failed to read input file\n"
# define E_UNDER_ENCRYPTION	"Error: wrong binary file! Binary file is already under our encryption\n"
# define E_USAGE			"Wrong syntax. Correct way is to type: ./woody_woodpacker ./path/binary or ./woody_woodpacker -i ./path/binary\n"
# define E_WRITE_WOODY		"Error: could not write to 'woody' file\n"

# define KEY_FORMAT			"key_value: %#.16llx\n"
# define CHUNK				4096

extern unsigned char	g_decryptor[];
extern unsigned int		g_decryptor_len;

typedef struct	s_decryption_set
{
	uint64_t	original_entry;
	uint64_t	encrypted_code;
	uint64_t	encrypted_size;
	uint64_t	key;
	uint64_t	encrypted_entry;
} t_dset;

typedef struct	s_woody
{
	ssize_t		filesize;
	void		*addr;
	Elf64_Ehdr	*ehdr;
	Elf64_Phdr	*code;
	Elf64_Phdr	*segments;
	Elf64_Shdr	*text;
	Elf64_Shdr	*sections;
	int			logs_fd;
	char		i_flag;
	int			res;
} t_woody;

int			another_keygenerator();
int			change_load_segment(t_dset *dset, t_woody *woody);
int			copy_info(t_woody *woody, int fd);
uint64_t	create_random_key(void);
int			detect_elftype(t_woody *woody);
int			encrypt_target(t_woody *woody);
uint64_t	encrypt_text_section(t_woody *woody, void *data, uint64_t key);
int			ft_memcmp(const void *memptr1, const void *memptr2, size_t num);
void		*ft_memmove(void *to, const void *from, size_t count);
void		*ft_memset(void *dest, int c, size_t len);
size_t		ft_strlen(const char *str);
int			ft_strncmp(const char *string1, const char *string2, size_t num);
ssize_t		get_filesize(t_woody *woody, int fd);
char		get_flags(char **av);
Elf64_Phdr	*get_load_segment(t_woody *woody);
Elf64_Shdr	*get_text_section(t_woody *woody);
size_t		get_zerofilled_size(t_woody *woody);
int			init_dset_struct(t_dset *dset, t_woody *woody, uint64_t key);
int			manage_file(t_woody *woody, char *filename);
int			open_file(t_woody *woody, char *filename);
int			prepare_to_write(t_woody *woody);
int			print_logs(void);
uint64_t	set_new_entry(t_woody *woody);
int			under_our_encryption(t_woody *woody);
int			write_to_file(t_woody *woody);

#endif
