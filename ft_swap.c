/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoaille <lnoaille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 22:08:17 by lnoaille          #+#    #+#             */
/*   Updated: 2021/06/16 15:06:41 by lnoaille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/time.h>
# include <fcntl.h>
# include <sys/stat.h> 
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdbool.h>
# include <semaphore.h>

unsigned long int	chrono_init(void)
{
	struct timeval		tv;
	unsigned long int	start_time;

	gettimeofday(&tv, NULL);
	start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (start_time);
}

unsigned long int	stamp_time(unsigned long int start_time)
{
	unsigned long int	actual_time;
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	actual_time = (tv.tv_sec * 1000 + tv.tv_usec / 1000) - start_time;
	return (actual_time);
}

int	ft_strlen(char *str)
{
	int i = 0;

	while(*str)
	{
		*str++;
		i++;
	}
	return (i);
}

int	ft_strlen_2(char *str)
{
	int i = 0;

	while(str[i])
	{
		i++;
	}
	return (i);
}

int ft_strlen_3(char *str)
{
	if (!*str)
		return (0);
	else
	{
		return (ft_strlen_3(++str)+ 1);
	}
}

int ft_strlen_4(char *str)
{
	if (!*str)
		return (0);
	else
	{
		return (ft_strlen_4(str + 1)+ 1);
	}
}

int main()
{
	char *str = "francoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisffrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancofrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisfrancoisis";


	unsigned long int timer = chrono_init();
	for (int i = 0; i < 300000; ++i)
	{
		ft_strlen_3(str);
	}
	unsigned long timer2 = stamp_time(timer);
	printf("recursion strlen = %lu\n", timer2);
	timer = chrono_init();
	for (int i = 0; i < 300000; i++)
	{
		ft_strlen_4(str);
	}
	timer2 = stamp_time(timer);
	printf("strlen with accessor = %lu\n", timer2);
}