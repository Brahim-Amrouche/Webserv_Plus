/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:49:43 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/09 22:50:40 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Response::ResponseException::ResponseException(const response_err &err, Response *cln)
    :TException("Response Error: ", err, cln)
{
    switch (err_c)
    {
        case E_FAILED_SEND:
            msg += "Failed to write to socket";
            break;
        case E_CLOSE_CONNECTION:
            msg += "Closing connection";
            break;
        default:
            msg += "Unknown error";
            break;
    }
}

Response::Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r): res_buf(buffer), req(r), cgi(buffer, req), 
    code(RES_NONE), buffer_size(0), res_header_sent(0) ,res_body_sent(0) , res_headers_done(false), res_body_done(false)
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

void Response::redirect(const deque<string> &redi_conf)
{
    cout << "Run to redirection man" << endl;
    stringstream ss;
    int i_code = 0;
    ss << redi_conf[0];
    response_code redir_code = PH::isRedirection_code(i_code);
    string status_line = RESH::getStatusLine(redir_code);
    RESH::pushHeaders(res_buf, status_line, buffer_size);
    string redirect_location = "Location: " + redi_conf[1] + "\r\n";
    RESH::pushHeaders(res_buf, redirect_location, buffer_size);
    pushDefaultHeaders();
    res_header_sent = buffer_size;
    res_headers_done= true;
}

void Response::serveFile(Path &path_dir)
{
    try
    {
        string content_type = RESH::getContentTypeHeader(path_dir);
        RESH::pushHeaders(res_buf, content_type, buffer_size);
        string content_length = RESH::getContentLengthHeader(path_dir);
        RESH::pushHeaders(res_buf, content_type, buffer_size);
        RESH::pushHeaders(res_buf, content_length, buffer_size);
        response_body = *path_dir;
        pushDefaultHeaders();
        res_header_sent = buffer_size;
        res_headers_done = true;
    }
    catch (const RESH::RESHException &e)
    {
        code = RES_UNAUTHORIZED;
        return;
    }
}

void Response::serveDirectory(Path &path_dir)
{
    deque<string> *autoindex = req[directives[AUTOINDEX]];
    deque<string> *index_page = req[directives[INDEX]];
    deque<string> *upload_dir = req[directives[UPLOAD_DIR]];
    if (autoindex && (*autoindex)[0] == "on")
    {
        cgi.setCgiMode(CGI_LIST_DIR);
        cgi << path_dir;
        return;
    }
    else if (index_page)
    {
        string &index = (*index_page)[0];
        Path index_path(*path_dir + index);
        if (index_path.isFile())
            return serveFile(index_path);
    }
    else if (req[UPLOAD_FILE] != "" && upload_dir && req.getReqMethod() == METHOD_POST 
        && req.getBodyMode() != M_NO_BODY)
    {
        Path upload_file(*path_dir + req[UPLOAD_FILE]);
        cgi.setCgiMode(CGI_UPLOAD);
        cgi << upload_file;
        return;
    }
    code = RES_NOT_FOUND;
    return;
}

void Response::generateResponse()
{
    if (res_headers_done == true)
        return ;
    deque<string> *cgi_active = req[directives[CGI]];
    deque<string> *redirection = req[directives[REDIRECTION]];
    if (redirection)
       return redirect(*redirection);
    if (cgi_active)
    {
        string &server_url = *req;
        Path cgi_path(server_url + req.getReqPath());
        cgi.setCgiMode(CGI_EXEC);
        cgi << cgi_path;
        return;
    }
    deque<string> *root = req[directives[ROOT]];
    Path req_path((*root)[0] + req.getReqPath());
    if (req_path.isFile())
        return serveFile(req_path);
    else if (req_path.isDir())
        return serveDirectory(req_path);
    else
    {
        code = RES_NOT_FOUND;
        return;
    }   
}

void Response::operator<<(const ssize_t &read_size)
{
    if (res_body_done)
        return ;
    ifstream file;
    file.open(response_body.c_str(), std::ios::binary);
    if (!file.is_open())
        throw ResponseException(E_FAILED_SEND, NULL);
    file.seekg(res_body_sent);
    file.read(res_buf + buffer_size, read_size);
    buffer_size += read_size;
    if (file.eof())
        res_body_done = true;
    file.close();
}

void Response::operator>>(Socket &client_sock)
{
    generateResponse();
    if (!res_headers_done)
        return ;
    ssize_t sent_size = 0;
    (*this) << HEADERS_MAX_SIZE - buffer_size;
    if ((sent_size = send(client_sock.getSockid(), res_buf, buffer_size, 0)) < 0)
        throw ResponseException(E_FAILED_SEND, NULL);
    FT::memmove(res_buf, res_buf + sent_size, buffer_size - sent_size);
    if (res_header_sent)
    {
        ssize_t temp = sent_size;
        buffer_size -= std::min(res_header_sent, (size_t)sent_size);
        sent_size -= std::min(res_header_sent, (size_t)sent_size);
        res_header_sent -= std::min(res_header_sent, (size_t)temp);
    }
    res_body_sent += sent_size;
    buffer_size -= sent_size;
    if (buffer_size == 0 && res_body_done)
        throw ResponseException(E_CLOSE_CONNECTION, NULL);
}

Response::~Response()
{}

