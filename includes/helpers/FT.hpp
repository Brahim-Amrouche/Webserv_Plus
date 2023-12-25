/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FT.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 12:43:17 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/25 12:53:19 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <cstddef>

class FT
{
    private :
        FT();
    public:
        static void	*memset(void *b, int c, size_t len);
        static void	*memcpy(void *dst, const void *src, size_t n);
        static size_t  strlen(const char *str);
};


