/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:49:43 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/09 15:54:27 by bamrouch         ###   ########.fr       */
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

Response::Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r): res_buf(buffer), req(r), cgi(buffer, req) ,code(RES_NONE), buffer_size(0)
{}

void Response::pushDefaultHeaders()
{
    string date = RESH::getDateHeader();
    RESH::pushHeaders(res_buf, date, buffer_size);
    string server = "Server: webserv/1.0\r\n";
    RESH::pushHeaders(res_buf, server, buffer_size);
    string close_connection = RESH::getCloseConnectionHeader();
    RESH::pushHeaders(res_buf, close_connection, buffer_size);
    RESH::pushHeaders(res_buf, "\r\n", buffer_size);
}

void Response::serveFile(Path &path_dir)
{
    string content_type = RESH::getContentTypeHeader(path_dir);
    RESH::pushHeaders(res_buf, content_type, buffer_size);
    try
    {
        string content_length = RESH::getContentLengthHeader(path_dir);
        RESH::pushHeaders(res_buf, content_type, buffer_size);
        RESH::pushHeaders(res_buf, content_length, buffer_size);
        file
        pushDefaultHeaders();
    }
    catch (const RESH::RESHException &e)
    {
        code = RES_UNAUTHORIZED;
        return;
    }
}

void Response::redirect(const deque<string> &redi_conf)
{
    stringstream ss;
    int i_code = 0;
    ss << redi_conf[0];
    response_code redir_code = PH::isRedirection_code(i_code);
    string status_line = RESH::getStatusLine(redir_code);
    FT::memmove(res_buf, status_line.c_str(), status_line.length());
    buffer_size += status_line.length();
    string redirect_location = "Location: " + redi_conf[1] + "\r\n";
    FT::memmove(res_buf + buffer_size, redirect_location.c_str(), redirect_location.length());
    buffer_size += redirect_location.length();
    pushDefaultHeaders();
    response_done = true;
}

void Response::findResponseFile(const Path &index_page)
{
    deque<string> *cgi_active = req[directives[CGI]];
    deque<string> *redirection = req[directives[REDIRECTION]];
    if (redirection)
       return redirect(*redirection);
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
        return serveFile(req_path);
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

