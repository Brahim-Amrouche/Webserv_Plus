/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESH.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 20:54:03 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/11 19:00:49 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void RESH::pushHeaders(char (&buffer)[HEADERS_MAX_SIZE + 1],const string &header, ssize_t &buffer_size)
{
    FT::memmove(buffer + buffer_size, header.c_str(), header.length());
    buffer_size += header.length();
}

string RESH::getStatusLine(const response_code &code)
{
    ostringstream oss;
    oss << "HTTP/1.1 " << code << " " << response_handle[code] << "\r\n";
    return (string(oss.str()));
}

string RESH::getStatusRedi(const response_code &code)
{
    ostringstream oss;
    
    oss << "Status: " << code << "\r\n";
    return (string(oss.str()));
}

string RESH::getCloseConnectionHeader()
{
    return ("Connection: close\r\n");
}

string RESH::getDateHeader()
{
    time_t rawtime;
    char buffer[100];

    time(&rawtime);
    strftime(buffer, 100, "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", gmtime(&rawtime));
    return (string(buffer));
}

string RESH::getContentLengthHeader(Path &file_path, ssize_t &res_body_size)
{
    ifstream file;
    string path = *file_path;
    file.open((*file_path).c_str(), std::ios::binary | std::ios::ate);
    if (!file.is_open())
        throw RESH::RESHException();
    ssize_t size = file.tellg();
    stringstream ss;
    if (!(ss << size) || size < 0)
        throw RESH::RESHException();
    res_body_size = size;
    return ("Content-Length: " + ss.str() + "\r\n");
}

string RESH::getContentTypeHeader(Path &content)
{
    string file = *content;
    size_t pos = file.find_last_of(".");
    string result = "Content-Type: ";
    if (pos == string::npos)
        return (result + mimetypes["default"] + "\r\n");
    string ext = file.substr(pos);
    if (mimetypes.find(ext) != mimetypes.end())
        return (result + mimetypes[ext] + "\r\n");
    return (result + mimetypes["default"] + "\r\n");
}
