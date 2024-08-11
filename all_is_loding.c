/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_is_loding.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 10:00:09 by aelkheta          #+#    #+#             */
/*   Updated: 2024/06/04 12:56:06 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Compiling.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:11:45 by aziz              #+#    #+#             */
/*   Updated: 2024/06/04 09:59:50 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char    *ft_free(char *buffer)
{
        free(buffer);
        return (NULL);
}

size_t  ft_strlcpy(char *dst, const char *src, size_t size)
{
        char    *dest;
        char    *str;
        size_t  i;

        i = 0;
        dest = (char *)dst;
        str = (char *)src;
        if (size > 0)
        {
                while (str[i] && i < size - 1)
                {
                        dest[i] = str[i];
                        i++;
                }
                dest[i] = '\0';
        }
        return (strlen(src));
}

char    *ft_strjoin(char *s1, char *s2)
{
        size_t  len_s1;
        size_t  len_s2;
        char    *ptr;

        if (!s1 && !s2)
                return (NULL);
        len_s1 = strlen(s1);
        len_s2 = strlen(s2);
        ptr = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
        if (!ptr)
                return (ft_free(s1));
        ft_strlcpy(ptr, s1, len_s1 + 1);
        ft_strlcpy((ptr + len_s1), s2, (len_s2 + 1));
        return (ptr);
}

void loding(char *s, int times)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int sleep_time = 80000;
    s = ft_strjoin(s, " ...");
    int len = strlen(s);

    while(k < times)
    {
        i = 0;
        while(i < len)
        {
            write(1, "\r", 1);
            write(1, s, i + 1);
            usleep(sleep_time);
            i++;
        }
        i = 0;
        while(i <= len)
        {
            write(1, "\r", 1);
            j = -1;
            while(++j < i)
                write(1, " ", 1);
            write(1, &s[i], strlen(&s[i]));
            usleep(sleep_time);
            i++;
        }
        k++;
    }
    free(s);
}

int main(int ac, char **av)
{
    if (ac != 3 && ac != 4)
    {
        printf("./a.out StrToLoding LodingTimes DoneMessage <==> [optional]\n");
        return 0;
    }
    loding(av[1], atoi(av[2]));
    if (ac == 4)
        printf("\r%s %s\n", av[1], av[3]);
    else
        printf("\r%s done successfully ✅\n", av[1]);

    return 0;
}
