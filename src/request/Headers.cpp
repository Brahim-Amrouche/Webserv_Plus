/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 21:02:47 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/16 18:37:18 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

const char *request_headers[6] = {
    "Host",
    "Content-Length",
    "Transfer-Encoding",
    "Content-Type",
    "Upload-File",
    "Cookie",
};

Headers::HeadersException::HeadersException(const headers_err &err, Headers *cln):TException("Headers Error: ", err, cln)
{
    switch (err_c)
    {
        case E_HEADERS_OVERFLOW:
            msg += "Headers Buffer Overflow";
            break;
        case E_DUPLICATE_HEADERS:
            msg += "Duplicate Headers";
            break;
        case E_INVALID_METHOD:
            msg+= "Invalid Request Method";
            break;
        case E_REQUEST_LINE:
            msg += "Request Line Error";
            break;
        case E_INVALID_HTTP_VERSION:
            msg += "Invalid HTTP Version";
            break;
        case E_NO_HOST_HEADER:
            msg += "No Host Header";
            break;
        default:
            msg += "Unknown Error";
            break;
    }   
}

Headers::Headers(char (&buffer)[HEADERS_MAX_SIZE + 1], ServerSocket &srv_sock ,ServerConfiguration *(&conf)):req_buffer(buffer), server_sock(srv_sock), 
    req_config(conf),req_method (METHOD_NONE) ,headers_done(false)
{}

void Headers::parseRequestLine()
{
    size_t pos = req_line.find_first_of(' ');
    if (pos == string::npos)
        throw HeadersException(E_REQUEST_LINE, NULL);
    string method = req_line.substr(0, pos);
    size_t pos2 = req_line.find_first_of(' ', pos + 1);
    if (pos == string::npos || pos2 == pos + 1)
        throw HeadersException(E_REQUEST_LINE, NULL);
    req_path = req_line.substr(pos + 1, pos2 - pos - 1);
    if (!PH::strIsPath(req_path))
        throw HeadersException(E_REQUEST_LINE, NULL);
    Path path_obj(req_path);
    req_config = (*req_config)[path_obj];
    ServerConfiguration *allowed_methods = (*req_config)[directives[ALLOW_METHODS]];
    deque<string>::iterator it = (**allowed_methods)->begin(), end = (**allowed_methods)->end();
    if (std::find(it, end, method) == end)
        throw HeadersException(E_INVALID_METHOD, NULL);
    req_method = REQH::getReqMethod(method);
    size_t pos3 = req_line.find_first_of('\r', pos2 + 1);
    if (pos3 == string::npos || pos3 == pos2 + 1)
        throw HeadersException(E_REQUEST_LINE, NULL);
    string version = req_line.substr(pos2 + 1, pos3 - pos2 - 1);
    if (version != "HTTP/1.1")
        throw HeadersException(E_INVALID_HTTP_VERSION, NULL);
}

void Headers::configureRequest()
{
    string host_string = (*this)[HOST];
    if (host_string.empty())
        throw HeadersException(E_NO_HOST_HEADER, NULL);
    req_config = server_sock[host_string]; 
    parseRequestLine();
}

bool Headers::operator<<(ssize_t &buffer_size)
{
    if (headers_done)
        return headers_done;
    size_t res = 0;
    try{
        if ((res = REQH::get_headers(req_buffer, buffer_size, req_line, headers)) < 0)
            return headers_done;
    }
    catch (const REQH::REQHException &e)
    {
        throw HeadersException(E_DUPLICATE_HEADERS , NULL);
    }
    if (res >= HEADERS_MAX_SIZE)
        throw HeadersException(E_HEADERS_OVERFLOW, NULL);
    buffer_size = buffer_size - res;
    headers_done = true;
    configureRequest();
    FT::memmove(req_buffer, req_buffer + res, buffer_size);
    return headers_done;
}

bool Headers::headersDone()
{
    return headers_done;
}

string Headers::operator[](const REQUEST_HEADERS &key)
{
    HeadersIt it = headers.find(request_headers[key]);
    if (it == headers.end())
        return "";
    return it->second;
}

void Headers::debugHeaders()
{
    HeadersIt it = headers.begin();
    while (it != headers.end())
    {
        std::cout << it->first << ": " << it->second << std::endl;
        ++it;
    }
}

Headers::~Headers()
{}