/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:28:12 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/03 19:19:16 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Path::Path()
{
    value = "";
}

Path::Path(const std::string &path)
{
    value = path;
}

void Path::operator--()
{
    size_t pos = value.find_last_of('/', 1);
    if (value == "/")
        value = "";
    if (pos == string::npos && value.size() > 1)
        value = "/";
    else
        value = value.substr(0, pos);
}

string Path::operator*()
{
    return value;
}

Path::~Path()
{}