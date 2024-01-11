/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:03:03 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/11 13:32:31 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Response.hpp"

enum CGI_MODE
{
    CGI_NONE,
    CGI_EXEC,
    CGI_UPLOAD,
    CGI_LIST_DIR,
};


class Cgi
{
    private:
        char (&buffer)[HEADERS_MAX_SIZE + 1];
        Request &req;
        CGI_MODE mode;
    public:
        Cgi(char (&b)[HEADERS_MAX_SIZE + 1], Request &r):buffer(b) ,req(r), mode(CGI_NONE) {};
        void setCgiMode(const CGI_MODE &m)
        {
            mode = m;
        };
        
        void operator<<(Path &path);
        ~Cgi();
};
