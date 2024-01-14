/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:49:43 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/14 20:12:18 by bamrouch         ###   ########.fr       */
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
        case E_FAILED_UPLOADING_FILE:
            msg += "Failed Uploading File";
            break;
        case E_CLOSE_CONNECTION:
            msg += "Closing connection";
            break;
        case E_FAILED_CGI_EXEC:
            msg += "Cgi Execution";
            break;
        default:
            msg += "Unknown error";
            break;
    }
}

Response::Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r, Client &cl): res_buf(buffer), req(r), client(cl)
    ,buffer_size(0), file(buffer_size) ,cgi(buffer, req, root_directory, file, buffer_size)
    ,res_headers_done(false), error_served(RES_NONE)
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
    cout << "Some type of error led us here|||||||||||||||" << endl;
    buffer_size = 0;
    string status_line = RESH::getStatusLine(err_code);
    RESH::pushHeaders(res_buf, status_line, buffer_size);
    Path content_ext(".txt");
    string content_type = RESH::getContentTypeHeader(content_ext);
    RESH::pushHeaders(res_buf, content_type, buffer_size);
    string content_length("Content-Length: 5\r\n");
    RESH::pushHeaders(res_buf, content_length, buffer_size);
    pushDefaultHeaders();
    string content("Error");
    FT::memmove(res_buf + buffer_size, content.c_str(), content.length());
    file.setFileDone(true);
    res_headers_done = true;
}

void Response::serveFile(Path &path_dir, const response_code &res_code)
{
    try
    {
        cout << "Going through here once" << endl;
        string status_line = RESH::getStatusLine(res_code);
        RESH::pushHeaders(res_buf, status_line, buffer_size);
        string content_type = RESH::getContentTypeHeader(path_dir);
        RESH::pushHeaders(res_buf, content_type, buffer_size);
        string content_length = RESH::getContentLengthHeader(path_dir, file.getFileSize());
        RESH::pushHeaders(res_buf, content_length, buffer_size);
        file.setFilePath(*path_dir);
        file.setFileDone(false);
        pushDefaultHeaders();
        res_headers_done = true;
        cgi.setCgiDone(true);
        // res_buf[buffer_size] = '\0';
        // cout << "the file is here :|" << *path_dir << "|" << endl;
        // cout << "the buffer is:|" << res_buf << "|" << endl;
    }
    catch (const RESH::RESHException &e)
    {
        return serveError(RES_UNAUTHORIZED);
    }
}

void Response::listDirectory(Path &path_dir)
{
    string req_path = req.getReqPath() == "" ? "/" : req.getReqPath();
    DIR *dir = opendir((*path_dir).c_str());
    if (!dir)
        return serveError(RES_UNAUTHORIZED);
    string status_line = RESH::getStatusLine(RES_OK);
    RESH::pushHeaders(res_buf, status_line, buffer_size);
    Path content_ext(".html");
    string content_type = RESH::getContentTypeHeader(content_ext);
    RESH::pushHeaders(res_buf, content_type, buffer_size);
    string html_page(DEFAULT_LIST_HTML);
    html_page += req_path;
    html_page += "</title></head><body><h1>Listing of ";
    html_page += req_path;
    html_page += "</h1><ul>";
    dirent *entry;
    while ((entry = readdir(dir)))
    {
        if (entry->d_name[0] == '.' && entry->d_name[1] == '\0')
            continue ;
        html_page += RESH::getHtmlListTag(req.getReqPath() + "/" + entry->d_name, entry->d_name);
    }
    html_page += "</ul></body></html>";
    closedir(dir);
    ostringstream oss;
    oss << html_page.length();
    string content_length("Content-Length: ");
    content_length += oss.str() + "\r\n";
    RESH::pushHeaders(res_buf, content_length, buffer_size);
    pushDefaultHeaders();
    if (buffer_size + html_page.length() > HEADERS_MAX_SIZE)
        return serveError(RES_INTERNAL_SERVER_ERROR);
    FT::memmove(res_buf + buffer_size, html_page.c_str(), html_page.length());
    file.setFileDone(true);
    res_headers_done = true;
    buffer_size += html_page.length();
}

void Response::serveDirectory(Path &path_dir)
{
    deque<string> *autoindex = req[directives[AUTOINDEX]];
    deque<string> *index_page = req[directives[INDEX]];
    deque<string> *upload_dir = req[directives[UPLOAD_DIR]];
    Path upload_path;
    if (upload_dir)
        upload_path = (*upload_dir)[0];
    if (req.getReqMethod() == METHOD_GET && path_dir.isDir() && autoindex && (*autoindex)[0] == "on")
        return listDirectory(path_dir);
    else if (req.getReqMethod() == METHOD_GET && path_dir.isDir() && index_page)
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
    else if (upload_path.isSubPath(req.getReqPath()))
    {
        // cout << "the upload Path is:|" << *upload_path << "|" << endl;
        // cout << "the req path is:|" << req.getReqPath() << "|" << endl;
        Path delete_file(root_directory + req.getReqPath());
        if (req.getReqMethod() == METHOD_POST && req.getBodyMode() != M_NO_BODY)
            return uploadFile();
        else if (req.getReqMethod() == METHOD_DELETE && delete_file.isFile())
        {
            remove((*delete_file).c_str());
            Path delete_res(DEFAULT_ROOT);
            delete_res += "/200.html";
            return serveFile(delete_res, RES_OK);
        }
    }
    return serveError(RES_NOT_FOUND);
}

void Response::uploadFile()
{
    string req_file(req.getReqId());
    string upload_dir(root_directory + req.getReqPath() + "/" + req[UPLOAD_FILE]);
    const char *tmp_file_path = req_file.c_str();
    const char *upload_file_path = upload_dir.c_str();
    // cout << "the tmpfile path is:|" << tmp_file_path << "|" << endl;
    // cout << "the upload file path is:|" << upload_file_path << "|" << endl;
    if (rename(tmp_file_path, upload_file_path) != 0)
        return serveError(RES_UNAUTHORIZED);
    Path upload_res(DEFAULT_ROOT);
    upload_res += "/201.html";
    return serveFile(upload_res, RES_CREATED);
    
}

void Response::generateResponse()
{
    if (res_headers_done == true)
        return ;
    deque<string> *cgi_active = req[directives[CGI]];
    deque<string> *redirection = req[directives[REDIRECTION]];
    root_directory = (*(req[directives[ROOT]]))[0];
    Path req_path(root_directory + req.getReqPath());
    CGI_LANG lang = L_UNKNOWN;
    Path cgi_script;
    cout << "the request path:|" << req.getReqPath()<< "|" << endl;
    cout << "with the whole thing being:|" << *req_path << "|" << endl;
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
    else if (cgi_active && (lang = req_path.isCgiPath(req.getReqPath(), cgi_script) ))
    {
        if ((*cgi_active)[0] == "php" && lang != L_PHP)
            return serveError(RES_FORBIDDEN);
        else if ((*cgi_active)[0] == "py" && lang != L_PYTHON)
            return serveError(RES_FORBIDDEN);
        Path full_path(req.getReqPath());
        cout << "the cgi path is:|" << *full_path << "| with lang is:" << lang << endl;
        cgi.setLang(lang);
        cgi.init(cgi_script, full_path);
        cgi.isDone();
        res_headers_done = true;
        return;
    }
    else if (req.getReqMethod() == METHOD_GET && req_path.isFile())
        return serveFile(req_path, RES_OK);
    else 
        return serveDirectory(req_path);
}


void Response::serveError(const response_code &err_code)
{
    buffer_size = 0;
    if (error_served)
        return serveErrorHeaders(error_served);
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
            serveFile(root_err_path + "403.html", error_served);
            break;
        case RES_NOT_FOUND:
            error_served = RES_NOT_FOUND;
            serveFile(root_err_path + "404.html", error_served);
            break;
        case RES_METHOD_NOT_ALLOWED:
            error_served = RES_METHOD_NOT_ALLOWED;
            serveFile(root_err_path + "405.html", error_served);
            break;
        case RES_REQUEST_TIMEOUT:
            error_served = RES_REQUEST_TIMEOUT;
            serveFile(root_err_path + "408.html", error_served);
            break;
        default:
            error_served = RES_INTERNAL_SERVER_ERROR;
            serveFile(root_err_path + "500.html", error_served);
    }
}



void Response::operator>>(Socket &client_sock)
{
    generateResponse();
    if (!res_headers_done || !cgi.isDone())
        return ;
    ssize_t sent_size = 0;
    if (!(*file))
        file >> res_buf;
    if ((sent_size = send(client_sock.getSockid(), res_buf, buffer_size, 0)) < 0)
        throw ResponseException(E_FAILED_SEND, NULL);
    client.setLastActivity();
    FT::memmove(res_buf, res_buf + sent_size, buffer_size - sent_size);
    buffer_size -= sent_size;
    if (buffer_size == 0 && *file)
        throw ResponseException(E_CLOSE_CONNECTION, NULL);
}

Response::~Response()
{}

