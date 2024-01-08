/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:03:03 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/08 16:39:54 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Response.hpp"


class Cgi
{
    private:
        char (&buffer)[HEADERS_MAX_SIZE + 1];
        Request &req;
    public:
        Cgi(char (&b)[HEADERS_MAX_SIZE + 1], Request &r):buffer(b) ,req(r) {};
        void operator<<(Path &path);
        ~Cgi();
};
