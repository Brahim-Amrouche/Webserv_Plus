/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:49:43 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/10 23:55:01 by bamrouch         ###   ########.fr       */
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
        case E_FAILED_RESPONSE_BODY_READ:
            msg += "Failed to read response body";
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
    buffer_size(0), res_headers_done(false), error_served(RES_NONE) ,file(buffer_size)
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

void Response::redirect(Path &redi_conf, const response_code &code)
{

    string status_line = RESH::getStatusLine(code);
    RESH::pushHeaders(res_buf, status_line, buffer_size);
    string redirect_location = "Location: " + *redi_conf + "\r\n";
    RESH::pushHeaders(res_buf, redirect_location, buffer_size);
    pushDefaultHeaders();
    file.setFileDone(true);
    res_headers_done= true;
}

void Response::serveErrorHeaders(const response_code &err_code)
{
    string status_line = RESH::getStatusLine(err_code);
    RESH::pushHeaders(res_buf, status_line, buffer_size);
    pushDefaultHeaders();
    file.setFileDone(true);
    res_headers_done = true;
}

void Response::serveFile(Path &path_dir, const response_code &res_code)
{
    try
    {
        string status_line = RESH::getStatusLine(res_code);
        RESH::pushHeaders(res_buf, status_line, buffer_size);
        string content_type = RESH::getContentTypeHeader(path_dir);
        RESH::pushHeaders(res_buf, content_type, buffer_size);
        string content_length = RESH::getContentLengthHeader(path_dir, file.getFileSize());
        RESH::pushHeaders(res_buf, content_length, buffer_size);
        file.setFilePath(*path_dir);
        pushDefaultHeaders();
        res_headers_done = true;
    }
    catch (const RESH::RESHException &e)
    {
        return serveError(RES_UNAUTHORIZED);
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
        string root_index_path = *path_dir;
        Path index_file_path(FT::discardLastSlash(root_index_path) + "/" + index);
        if (index_file_path.isFile())
        {
            Path index_route(req.getReqPath() + "/" + index);
            cout << "it reached here *:|" << *index_route << "|" << endl;
            return redirect(index_route, RES_FOUND);
        }
    }
    else if (req[UPLOAD_FILE] != "" && upload_dir && req.getReqMethod() == METHOD_POST 
        && req.getBodyMode() != M_NO_BODY)
    {
        Path upload_file(*path_dir + req[UPLOAD_FILE]);
        cgi.setCgiMode(CGI_UPLOAD);
        cgi << upload_file;
        return;
    }
    return serveError(RES_NOT_FOUND);
}

void Response::generateResponse()
{
    if (res_headers_done == true)
        return serveErrorHeaders(RES_INTERNAL_SERVER_ERROR);
    deque<string> *cgi_active = req[directives[CGI]];
    deque<string> *redirection = req[directives[REDIRECTION]];
    if (redirection)
    {
        stringstream ss;
        int i_code = 0;
        ss << (*redirection)[0];
        ss >> i_code;
        response_code code = PH::isRedirection_code(i_code);
        Path redi_path((*redirection)[1]);
        return redirect(redi_path, code);
    }
    if (cgi_active)
    {
        string &server_url = *req;
        Path cgi_path(server_url + req.getReqPath());
        cgi.setCgiMode(CGI_EXEC);
        cgi << cgi_path;
        return;
    }
    root_directory = (*(req[directives[ROOT]]))[0];
    Path req_path(root_directory + req.getReqPath());
    if (req_path.isFile())
        return serveFile(req_path, RES_OK);
    else if (req_path.isDir())
        return serveDirectory(req_path);
    else
        return serveError(RES_NOT_FOUND);
}


void Response::serveError(const response_code &err_code)
{
    if (error_served)
        return ;
    buffer_size = 0;
    deque<string> *err_page = req[directives[ERROR_PAGE]];
    if (err_page)
    {
        int i_code = static_cast<int>(err_code);
        stringstream ss;
        ss << i_code;
        deque<string>::iterator result = std::find(err_page->begin(), err_page->end(), ss.str());
        if (result != err_page->end())
        {
            while (!PH::strIsPath(*(++result)))
                ;
            Path err_redir(*result);
            return redirect(err_redir , RES_FOUND);
        }
    }
    Path root_err_path(DEFAULT_ERROR_PAGES);
    root_err_path += "/";
    buffer_size = 0;
    switch (err_code)
    {
        case RES_BAD_REQUEST:
            error_served = RES_BAD_REQUEST;
            serveFile(root_err_path + "400.html", error_served);
            break;
        case RES_UNAUTHORIZED:
            error_served = RES_UNAUTHORIZED;
            serveFile(root_err_path + "401.html", error_served);
            break;
        case RES_FORBIDDEN:
            error_served = RES_FORBIDDEN;
            serveFile(root_err_path + "403.html", RES_FORBIDDEN);
            break;
        case RES_NOT_FOUND:
            error_served = RES_NOT_FOUND;
            serveFile(root_err_path + "404.html", error_served);
            break;
        case RES_METHOD_NOT_ALLOWED:
            error_served = RES_METHOD_NOT_ALLOWED;
            serveFile(root_err_path + "405.html", error_served);
            break;
        default:
            serveFile(root_err_path + "500.html", RES_INTERNAL_SERVER_ERROR);
    }
}

void Response::operator>>(Socket &client_sock)
{
    generateResponse();
    if (!res_headers_done)
        return ;
    ssize_t sent_size = 0;
    if (!(*file))
        file >> res_buf;
    if ((sent_size = send(client_sock.getSockid(), res_buf, buffer_size, 0)) < 0)
        throw ResponseException(E_FAILED_SEND, NULL);
    FT::memmove(res_buf, res_buf + sent_size, buffer_size - sent_size);
    buffer_size -= sent_size;
    cout << "the buffer size after send is: " << buffer_size << endl; 
    if (buffer_size == 0 && *file)
        throw ResponseException(E_CLOSE_CONNECTION, NULL);
}

Response::~Response()
{}

