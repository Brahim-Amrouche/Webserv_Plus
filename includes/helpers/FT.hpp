/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FT.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 12:43:17 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/05 19:28:00 by bamrouch         ###   ########.fr       */
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
        static void *memmove(void *dst, const void *src, size_t len);
        static void *memchr(const void *src, void *chr, size_t n, size_t m);
        static size_t  strlen(const char *str);
        static bool    strIsDigit(const string &str);
        static string  strToLowercase(const string &str);
};


