/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:28:12 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/07 17:18:23 by bamrouch         ###   ########.fr       */
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

Path::isDir()
{
    struct stat st;
    if (stat(value.c_str(), &st) == 0)
        return S_ISDIR(st.st_mode);
    return false;

}

void Path::operator--()
{
    size_t pos = value.find_last_of('/');
    if (value == "/")
        value = "";
    else
        value = value.substr(0, pos ? pos : 1);
}

string Path::operator*()
{
    return value;
}

Path::~Path()
{}