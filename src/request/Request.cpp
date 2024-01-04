/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:48:01 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/05 00:18:22 by bamrouch         ###   ########.fr       */
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

Request::Request(Socket &client_sock, ServerSocket &server_sock):req_id(REQH::generateReqId()) , buffer_size(0), client_sock(client_sock), server_sock(server_sock), server_config(NULL)
    ,headers(req_buffer, server_sock, server_config)
{}

Request::Request(const Request &cpy_req):req_id(cpy_req.req_id), buffer_size(cpy_req.buffer_size), client_sock(cpy_req.client_sock) ,server_sock(cpy_req.server_sock), server_config(cpy_req.server_config), headers(req_buffer, server_sock, server_config)
{}

Request &Request::operator=(const Request &eq_req)
{
    if (this != &eq_req)
    {
        req_id = eq_req.req_id;
        buffer_size = eq_req.buffer_size;
        client_sock = eq_req.client_sock;
        server_sock = eq_req.server_sock;
        server_config = eq_req.server_config;
    }
    return *this;
}

// void Request::readBody()
// {
//     std::istringstream iss(req_headers["Content-Length"]);
//     ssize_t content_length = 0;
//     if (!(iss >> content_length))
//         throw RequestException(E_BODY_SIZE_OVERFLOW, NULL);
//     if (body_size == content_length)
//         body_done = true;
//     if (counter < 0 || !headers_done || body_done)
//         return;
//     if (counter > 0)
//     {
//         *this << req_buffer;
//         body_size += counter;
//     } 
//     // if (body_size > content_length)
//     //     throw RequestException(E_BODY_SIZE_OVERFLOW, NULL);
//     if (body_size >= content_length)
//     {
//         cout << "done from b" << endl;
//         body_done = true;
//     }
// }

void Request::read()
{
    ssize_t temp = 0;
    if (headers.headersDone())
        buffer_size = 0;
    if ((temp = recv(client_sock.getSockid(), req_buffer + buffer_size, HEADERS_MAX_SIZE - buffer_size, 0)) <= 0)
        throw RequestException(E_FAILED_READ, NULL);
    buffer_size += temp;
    req_buffer[buffer_size] = '\0';
    try
    {
        cout << "buffer size before: "<< buffer_size << endl;
        if ((headers << buffer_size))
        {
            cout << "buffer size after: "<< buffer_size << endl;
            headers.debugHeaders();
            server_config->debug_print_directives();
            throw RequestException(E_READING_DONE, NULL);
        }
    }
    catch (const Headers::HeadersException &e)
    {
        cout << e.what() << endl;
        throw RequestException(E_FAILED_HEADERS_READ, NULL);
    }
}

string Request::operator[](const REQUEST_HEADERS &key)
{
    return headers[key];
}

// void Request::operator<<(const char *buffer)
// {
//     ofstream file(req_id.c_str(), std::ios::app | std::ios::binary);
    
//     if (!file.is_open())
//         throw RequestException(E_FAILED_BODY_WRITE, this);
//     file.write(buffer, counter);
//     file.close();
// }

Request::~Request()
{
    if (req_id.size() > 0)
        remove(req_id.c_str());
}