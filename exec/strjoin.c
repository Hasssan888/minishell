/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 13:27:06 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/10 13:27:42 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

char	*strjoin1(char *s1, char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*ptr;

	if (!s1 && !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	ptr = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!ptr)
		return (ft_free(s1));
	ft_strlcpy(ptr, s1, len_s1 + 1);
	ft_strlcpy((ptr + len_s1), s2, (len_s2 + 1));
	return (ptr);
}
