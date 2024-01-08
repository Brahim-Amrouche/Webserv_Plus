/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:49:43 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/08 21:16:46 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Response::ResponseException::ResponseException(const response_err &err, Response *cln)
    :TException("Response Error: ", err, cln)
{
    switch (err)
    {
        case E_FAILED_WRITE:
            this->msg += "Failed to write to socket";
            break;
        default:
            this->msg += "Unknown error";
            break;
    }
}

Response::Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r): res_buf(buffer), req(r), cgi(buffer, req) ,code(NONE), buffer_size(0)
{}

void Response::findResponseFile(const Path &index_page)
{
    deque<string> *cgi_active = req[directives[CGI]];
    deque<string> *redirection = req[directives[REDIRECTION]];
    if (redirection)
    {
        
    }
    if (cgi_active)
    {
        string &server_url = *req;
        Path cgi_path(req.getReqPath());
        cgi << cgi_path;
        return;
    }
    deque<string> *root = req[directives[ROOT]];
    Path req_path((*root)[0] + req.getReqPath());
    if (req_path.isFile())
    {
        response_path = *req_path;
        return;
    }
    else if (req_path.isDir())
    {
        deque<string> *autoindex = req[directives[AUTOINDEX]];
        deque<string> *index_page = req[directives[INDEX]];
        if (autoindex && (*autoindex)[0] == "on")
        {
            listDirectory();
            return;
        }
        else if (index_page && )
        {
            Path index_path(*req_path + (*index_page)[0]);
            if (index_path.isFile())
            {
                response_path = *index_path;
                return;
            }
        }
        return;
    }
    else
    {
        code = RES_NOT_FOUND;
        return;
    }
    
}

void Response::generateResponse()
{
    findResponseFile();
}

bool Response::operator>>(Socket &client_sock)
{
    (void)client_sock;
    (void)res_buf;
    (void)req;
    (void)code;
    (void)buffer_size;
    generateResponse();
    // if (write(client_sock.getSockid(), response.c_str(), response.length()) < 0)
    //     throw ResponseException(E_FAILED_WRITE, this);
    return (true);
}

Response::~Response()
{}

