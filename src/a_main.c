/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 10:33:53 by abambi            #+#    #+#             */
/*   Updated: 2022/09/07 19:02:59 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	main(int ac, char **av)
{
	t_woody	woody;
	int		ret;

	ret = 0;
	ft_memset(&woody, 0, sizeof(woody));
	woody.logs_fd = open("log.txt", O_WRONLY|O_TRUNC|O_CREAT);
	if (ac != 2 && ac != 3 )
	{
		write(2, E_USAGE, 110);
		close(woody.logs_fd);
		return (1);
	}
	if (ac == 3)
	{
		woody.i_flag = get_flags(av);
	}
	if (manage_file(&woody, av[ac - 1]))
		ret = 1;
	close(woody.logs_fd);
	return (ret);
}

char	get_flags(char **av)
{
	if (ft_strlen(av[1]) == 2 && \
		av[1][0] == '-' && av[1][1] == 'i')
		return ('i');
	else
	{
		write(2, E_FLAG, 41);
		return (0);
	}
}
