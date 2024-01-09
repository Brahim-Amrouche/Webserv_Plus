/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 21:48:20 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/09 21:51:19 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Cgi::operator<<(Path &path)
{
    (void)path;
    (void) buffer;
    (void)req;
    (void)mode;
    return;
}


Cgi::~Cgi()
{
    // if (mode == CGI_EXEC)
    //     remove(req.getUploadFile().c_str());
}