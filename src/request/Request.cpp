/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:48:01 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/02 23:38:28 by bamrouch         ###   ########.fr       */
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
        case E_REQ_BUFFER_OVERFLOW:
            msg += "Request Buffer Overflow";
            break;
        case E_DUPLICATE_HEADERS:
            msg += "Duplicate Headers";
            break;
        case E_READING_DONE:
            msg += "Reading Done";
            break;
        default:
            msg += "Unknown Error";
    }
}

Request::Request(Socket &client_sock, ServerSocket &server_sock): client_sock(client_sock), server_sock(server_sock),
    headers_size(0), server_config(NULL), headers_done(false), body_done(false)
{}

Request::Request(const Request &cpy_req):client_sock(cpy_req.client_sock), server_sock(cpy_req.server_sock),
    headers_size(cpy_req.headers_size), server_config(cpy_req.server_config), headers_done(cpy_req.headers_done), body_done(cpy_req.body_done)
{}

Request &Request::operator=(const Request &eq_req)
{
    if (this != &eq_req)
    {
        client_sock = eq_req.client_sock;
        server_sock = eq_req.server_sock;
        headers_size = eq_req.headers_size;
        server_config = eq_req.server_config;
        headers_done = eq_req.headers_done;
        body_done = eq_req.body_done;
    }
    return *this;
}

void Request::readHeaders()
{
    if (headers_done)
        return;
    headers_size += counter;
    req_buffer[headers_size] = '\0';
    try
    {
        if ((counter = REQH::get_headers(req_buffer , headers_size, req_headers)) < 0)
            return;
    }
    catch(const REQH::REQHException &e)
    {
        cout << e.what() << endl;
        throw RequestException(E_DUPLICATE_HEADERS, NULL);
    }
    if (counter >= REQ_BUFFER_SIZE)
        throw RequestException(E_REQ_BUFFER_OVERFLOW, NULL);
    req_id = REQH::generateReqId();
    headers_done = true;
    if (counter < headers_size)
    {
        ssize_t temp = counter;
        counter = headers_size - counter;
        *this << (req_buffer + temp);
        headers_size = 0;
        body_size = counter;
    }
    counter = -1;

}

void Request::readBody()
{
    std::istringstream iss(req_headers["Content-Length"]);
    ssize_t content_length = 0;
    iss >> content_length;
    if (body_size == content_length)
        body_done = true;
    if (counter < 0 || !headers_done || body_done)
        return;
    if (counter > 0)
    {
        *this << req_buffer;
        body_size += counter;
    } 
    // cout << "content length is: " << req_headers["Content-Length"] << endl; 
    // cout << "i is: " << body_size << endl;
    if (body_size > content_length)
    {
        cout << "done from a" << endl;
        body_done = true;
    }
    else if (body_size == content_length)
    {
        cout << "done from b" << endl;
        body_done = true;
    }
}

void Request::read()
{
    if ((counter = recv(client_sock.getSockid(), req_buffer + headers_size, REQ_BUFFER_SIZE - headers_size, 0)) < 0)
        throw RequestException(E_FAILED_READ, this);
    readHeaders();
    // debugHeaders();

    readBody();
    // cout << "hello i read and it was ============= "  << counter << " bytes" << endl;
    // throw RequestException(E_READING_DONE, NULL);
}

void Request::debugHeaders()
{
    HeadersIt it = req_headers.begin();
    while (it != req_headers.end())
    {
        std::cout << it->first << " : " << it->second << std::endl;
        it++;
    }
}

void Request::operator<<(const char *buffer)
{
    ofstream file(req_id.c_str(), std::ios::app | std::ios::binary);
    
    if (!file.is_open())
        throw RequestException(E_FAILED_BODY_WRITE, this);
    file.write(buffer, counter);
    file.close();
}

Request::~Request()
{
    if (req_id.size() > 0)
        remove(req_id.c_str());
}