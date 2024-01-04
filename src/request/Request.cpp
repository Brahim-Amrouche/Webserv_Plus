/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:48:01 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/04 17:57:28 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

const char *request_headers[3] = {
    "Host",
    "Content-Length",
    "Transfer-Encoding",
};

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
        case E_NO_HOST_HEADER:
            msg += "No Host Header";
            break;
        case E_REQUEST_LINE:
            msg += "Request Line Error";
            break;
        case E_INVALID_METHOD:
            msg+= "Invalid Request Method";
            break;
        case E_INVALID_HTTP_VERSION:
            msg += "Invalid HTTP Version";
            break;
        case E_BODY_SIZE_OVERFLOW:
            msg += "Body Size Overflow";
            break;
        case E_READING_DONE:
            msg += "Reading Done";
            break;
        default:
            msg += "Unknown Error";
    }
}

Request::Request(Socket &client_sock, ServerSocket &server_sock):client_sock(client_sock), server_sock(server_sock),
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

void Request::parseRequestLine()
{
    size_t pos = req_method.find(' ');
    if (pos == string::npos)
        throw RequestException(E_REQUEST_LINE, NULL);
    string method = req_method.substr(0, pos);
    req_method = req_method.substr(pos + 1);
    size_t pos2 = req_method.find_first_of(' ');
    if (pos == string::npos || pos2 == pos + 1)
        throw RequestException(E_REQUEST_LINE, NULL);
    string path = req_method.substr(0, pos2);
    if (path[path.size() -1] == '/')
        path.erase(path.size() - 1 ,1);
    // Checking Method;
    Path path_value(path);
    server_config = (*server_config)[path_value];
    ServerConfiguration *allowed_methods = (*server_config)[directives[ALLOW_METHODS]];
    deque<string>::iterator it = (**allowed_methods)->begin(), end = (**allowed_methods)->end();
    if (std::find(it, end, method) == end)
        throw RequestException(E_INVALID_METHOD, NULL);
    req_method = req_method.substr(pos2 + 1);
    size_t pos3 = req_method.find_first_of('\r', pos2 + 1);
    if (pos3 == string::npos || pos3 == pos2 + 1)
        throw RequestException(E_REQUEST_LINE, NULL);
    // Checking HTTP Version
    string version = req_method.substr(0, pos3);
    if (version != "HTTP/1.1")
        throw RequestException(E_REQUEST_LINE, NULL);
    throw  RequestException(E_READING_DONE, NULL);
}

void Request::configureRequest()
{
    HeadersIt it = req_headers.end();
    if ( (*this)[HOST].size() == 0)
        throw RequestException(E_NO_HOST_HEADER, NULL);
    Path host_value(it->second);
    server_config = server_sock[host_value];
    parseRequestLine();
}

void Request::readHeaders()
{
    if (headers_done)
        return;
    headers_size += counter;
    req_buffer[headers_size] = '\0';
    try
    {
        if ((counter = REQH::get_headers(req_buffer , headers_size, req_method, req_headers)) < 0)
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
    configureRequest();
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
    if (!(iss >> content_length))
        throw RequestException(E_BODY_SIZE_OVERFLOW, NULL);
    if (body_size == content_length)
        body_done = true;
    if (counter < 0 || !headers_done || body_done)
        return;
    if (counter > 0)
    {
        *this << req_buffer;
        body_size += counter;
    } 
    // if (body_size > content_length)
    //     throw RequestException(E_BODY_SIZE_OVERFLOW, NULL);
    if (body_size >= content_length)
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
    readBody();
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

string Request::operator[](const REQUEST_HEADERS &key)
{
    HeadersIt it = req_headers.find(request_headers[key]);
    if (it == req_headers.end())
        return "";
    return it->second;
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