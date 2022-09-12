# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abambi <abambi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/23 19:20:21 by abambi            #+#    #+#              #
#    Updated: 2022/09/10 11:15:08 by abambi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= woody_woodpacker

CFLAGS	+= -Wall -Werror -Wextra
CFLAGS	+= -Iinclude

SRCDIR	= src
SRC		= \
			a_main.c \
			b_read_file.c \
			c_prepare_to_write.c \
			d_encrypt_code.c \
			e_write_to_file.c \
			utils_lib.c \

OBJDIR	= obj
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

.PHONY: all clean flean re makedir log

all: makedir $(NAME)

makedir:
	@if [ ! -d $(OBJDIR) ]; then mkdir -p $(OBJDIR); fi

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/g_decryptor.c:
	nasm -f bin $(SRCDIR)/inject.s -o $(OBJDIR)/g_decryptor
	cd $(OBJDIR) && xxd -i -c8 g_decryptor g_decryptor.c

$(NAME): $(OBJ) $(OBJDIR)/g_decryptor.o include/woody.h
	$(CC) $(CFLAGS) $(OBJ) $(OBJDIR)/g_decryptor.o -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean:	clean
	rm -f $(NAME)
	rm -f woody
	rm -f log.txt

re: fclean all

log:
	@cat log.txt
