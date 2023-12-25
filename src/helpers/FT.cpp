/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FT.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 12:46:06 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/25 14:16:24 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

size_t FT::strlen(const char *str)
{
    if (!str)
        return 0;
    size_t i = 0;
    while (str[i])
        ++i;
    return i;
}

void *FT::memset(void *b, int c, size_t len)
{
    size_t	i = -1;
	while (++i < len)
		((unsigned char *)b)[i] = (unsigned char)c;
	return (b);
}

void *FT::memcpy(void *dst, const void *src, size_t n)
{
    unsigned char	*c_dst;
	unsigned char	*c_src;
	size_t	i;

	if (!dst && !src && n)
		return (dst);
	c_dst = (unsigned char *)dst;
	c_src = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		c_dst[i] = c_src[i];
		i++;
	}
	return (dst);
}

