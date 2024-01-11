/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 21:48:20 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/11 13:39:08 by bamrouch         ###   ########.fr       */
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
{}