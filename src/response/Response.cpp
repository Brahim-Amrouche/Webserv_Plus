/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:49:43 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/07 17:36:11 by bamrouch         ###   ########.fr       */
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

Response::Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r): res_buf(buffer), req(r), code(NONE), buffer_size(0)
{}

void 

void Response::findResponseFile(const Path &index_page)
{
    deque<string> *index = req[directives[INDEX]];
    deque<string> *root = req[directives[ROOT]];
    deque<string> *autoindex = req[directives[AUTOINDEX]];
    deque<string> *cgi = req[directives[CGI]];
    deque<string> *redirection = req[directives[REDIRECTION]];
    Path req_path((*root)[0] + req.getReqPath());
    
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

