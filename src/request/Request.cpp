/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:48:01 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/13 01:53:50 by bamrouch         ###   ########.fr       */
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
        {
            // cout << " this is the server configuration: ===============================" << endl;
            // server_config->debug_print_directives();
            // cout << "====================================================================" << endl;
            return;
        }
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

string &Request::operator*()
{
    deque<string> &location_name = ***server_config;
    return location_name[1];
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
    remove(req_id.c_str());
}