/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:48:01 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/07 11:22:55 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Request::RequestException::RequestException(const request_err &err, Request *cln): TException("Request Error: ", err, cln) 
{
    switch (err_c)
    {
        case E_FAILED_READ:
            msg += "Failed To Read";
            break;
        case E_FAILED_HEADERS_READ:
            msg += "Failed To Read Headers";
            break;
        case E_FAILED_BODY_READ:
            msg += "Failed To Write Body";
            break;
        case E_READING_DONE:
            msg += "Reading Done";
            break;
        default:
            msg += "Unknown Error";
    }
}

Request::Request(char (&buffer)[HEADERS_MAX_SIZE + 1], Socket &c_sock, ServerSocket &s_sock):req_id(REQH::generateReqId()), req_buffer(buffer)
    , buffer_size(0), client_sock(c_sock), server_sock(s_sock), server_config(NULL)
    , headers(req_buffer, server_sock, server_config), body(req_id, req_buffer, server_config, headers)
{}

// Request::Request(const Request &cpy_req):req_id(cpy_req.req_id), buffer_size(cpy_req.buffer_size)
//     , client_sock(cpy_req.client_sock) ,server_sock(cpy_req.server_sock)
//     , server_config(cpy_req.server_config), headers(req_buffer, server_sock, server_config)
//     , body(req_id, req_buffer, server_config, headers)
// {}

// Request &Request::operator=(const Request &eq_req)
// {
//     if (this != &eq_req)
//     {
//         req_id = eq_req.req_id;
//         buffer_size = eq_req.buffer_size;
//         client_sock = eq_req.client_sock;
//         server_sock = eq_req.server_sock;
//         server_config = eq_req.server_config;
//     }
//     return *this;
// }

void Request::read()
{
    if (headers.headersDone() && body.bodyDone())
        throw RequestException(E_READING_DONE, NULL);
    ssize_t temp = 0;
    if ((temp = recv(client_sock.getSockid(), req_buffer + buffer_size, HEADERS_MAX_SIZE - buffer_size, 0)) < 0)
        throw RequestException(E_FAILED_READ, NULL);
    buffer_size += temp;
    req_buffer[buffer_size] = '\0';
    try
    {
        if ((headers << buffer_size) && (body << buffer_size))
            return;
    }
    catch (const Headers::HeadersException &e)
    {
        cout << e.what() << endl;
        throw RequestException(E_FAILED_HEADERS_READ, NULL);
    }
    catch (const Body::BodyException &e)
    {
        cout << e.what() << endl;
        throw RequestException(E_FAILED_BODY_READ, NULL);
    }
}

deque<string> *Request::operator[](const string &server_key)
{
    if (!server_config)
        return NULL;
    ServerConfiguration *conf = (*server_config)[server_key];
    if (!conf)
        return NULL;
    return **conf;
}

string Request::operator[](const REQUEST_HEADERS &key)
{
    return headers[key];
}

Request::~Request()
{
    if (req_id.size() > 0)
        remove(req_id.c_str());
}