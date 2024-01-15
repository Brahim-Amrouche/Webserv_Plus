/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:23:26 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/15 22:42:12 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Body::BodyException::BodyException(const body_err &err, Body *cln): TException("Body Error: ", err, cln)
{
    switch (err_c)
    {
        case E_INVALID_BODY_CONFIG:
            msg += "Invalid Body Config";
            break;
        case E_INVALID_BODY_HEADERS:
            msg += "Invalid Body Headers";
            break;
        case E_BODY_SIZE_OVERFLOW:
            msg += "Body Overflow";
            break;
        case E_BODY_READING:
            msg += "Body Reading Failed";
            break;
        case E_UNABLE_TO_OPEN_TMP_FILE:
            msg += "Unable to open tmp file";
            break;
        default :
            msg += "Unknown Error";
    }
}

Body::Body(string &id, char (&buf)[HEADERS_MAX_SIZE + 1], ServerConfiguration *(&c), Headers &h)
    :req_id(id), buffer(buf),conf(c), headers(h) ,mode(M_NO_CONF), body_done(false) 
{
    content_length = 0;   
    body_size = 0;
    max_config_size = LLONG_MAX;
}

void Body::fromConfig()
{
    ServerConfiguration *client_max_body = (*conf)[directives[CLIENT_MAX_BODY_SIZE]];
    if (!client_max_body)
        return;
    deque <string> *config_values = **client_max_body;
    const char *start = config_values->front().c_str();
    char *end = NULL;
    max_config_size = strtol(start, &end, 10);
    if (end == start || *end != '\0')
        throw BodyException(E_INVALID_BODY_CONFIG, NULL);
}

void Body::fromHeaders()
{
    cout << "counfiguring it" << endl;
    if (headers[TRANSFER_ENCODING] != "")
    {
        if (headers[TRANSFER_ENCODING] == "chunked")
        {
            content_length = -2;
            mode = M_CHUNKED;
        }
        else if (headers[TRANSFER_ENCODING] == "identity")
            mode = M_IDENTITY;
        else
            throw BodyException(E_INVALID_BODY_HEADERS, NULL);
    }
    if (headers[CONTENT_LENGTH] != "" && mode == M_CHUNKED)
        throw BodyException(E_INVALID_BODY_HEADERS, NULL);
    else if (headers[CONTENT_LENGTH] != "" || mode == M_IDENTITY)
    {
        string content_lenght_header = headers[CONTENT_LENGTH];
        const char *start = content_lenght_header.c_str();
        char *end = NULL;
        content_length = strtol(start, &end, 10);
        cout << "the content length is: " << content_length << endl;;
        if (end == start || *end != '\0' || content_length <= 0 || content_length > max_config_size)
            throw BodyException(E_INVALID_BODY_HEADERS, NULL);
        mode = M_CONTENT_LENGTH;
    }
    else if (mode == M_NO_CONF)
        mode = M_NO_BODY;
}

void Body::configBody()
{
    fromConfig();
    fromHeaders();
    if (mode != M_NO_BODY && headers[CONTENT_TYPE] == "")
        throw BodyException(E_INVALID_BODY_HEADERS, NULL);
    else if (mode == M_NO_BODY && headers[CONTENT_TYPE] != "")
        throw BodyException(E_INVALID_BODY_HEADERS, NULL);
}

bool Body::readChunked(ssize_t &buffer_size)
{
    if (body_done)
        return body_done;
    try
    {
        if (content_length == -2
            && !REQH::CRLF_found(buffer, buffer_size, &content_length))
            return false;
        if (content_length + body_size > max_config_size)
            throw BodyException(E_BODY_SIZE_OVERFLOW, NULL);
    }
    catch(const REQH::REQHException& e)
    {
        throw BodyException(E_BODY_READING, NULL);
    }
    ssize_t write_bytes = std::min(content_length == -2 ? 0: content_length, buffer_size);
    body_file.write(buffer, write_bytes);
    if (write_bytes == content_length)
        FT::memmove(buffer, buffer + write_bytes, buffer_size - write_bytes);
    buffer_size -= write_bytes;
    content_length -= write_bytes;
    body_size += write_bytes;
    if (buffer_size == 0)
        return false;
    ssize_t temp = buffer_size;
    if (REQH::CRLF_found(buffer, buffer_size, NULL))
    {
        content_length = -2;
        if (write_bytes == 0 && buffer_size == 0)
            return true;
        else if (buffer_size != temp - 2)
            throw BodyException(E_BODY_READING, NULL);
        else if (buffer_size > 0)
            return readChunked(buffer_size);
    }
    else
    {
        if (buffer_size >= 2)
            throw BodyException(E_BODY_READING, NULL);
        return false;
    }
    return false;
}

// bool Body::readIdentity(ssize_t &buffer_size)
// {
//     if (body_size + buffer_size > max_config_size)
//         throw BodyException(E_BODY_SIZE_OVERFLOW, NULL);
//     body_file.write(buffer, buffer_size);
//     body_size += buffer_size;
//     buffer_size = 0;
//     return false;
// }

bool Body::readContentLength(ssize_t &buffer_size)
{
    if (body_done)
        return true;
    if (body_size + buffer_size > content_length 
        || body_size + buffer_size > max_config_size)
        throw BodyException(E_BODY_SIZE_OVERFLOW, NULL);
    cout <<"In Content_Length" << endl;
    body_file.write(buffer, buffer_size);
    body_size += buffer_size;
    buffer_size = 0;
    return body_size == content_length;
}

bool Body::operator<<(ssize_t &buffer_size)
{
    if (buffer_size == 0)
        return (body_done = true);
    body_file.open(req_id.c_str(), std::ios::app | std::ios::binary);
    if (!body_file.is_open())
        throw BodyException(E_UNABLE_TO_OPEN_TMP_FILE, NULL);
    if (mode == M_NO_CONF)
        configBody();
    if (mode == M_NO_BODY && buffer_size != 0)
        throw BodyException(E_INVALID_BODY_HEADERS, NULL);
    switch (mode)
    {
        case M_NO_CONF:
            break;
        case M_NO_BODY:
            body_done = true;
            break;
        case M_CHUNKED:
            body_done = readChunked(buffer_size);
            break;
        case M_IDENTITY:
        case M_CONTENT_LENGTH:
            body_done = readContentLength(buffer_size);
            break;
    }
    body_file.close();
    return body_done;
}

bool Body::bodyDone()
{
    return body_done;
}

Body::~Body()
{
    if (body_file.is_open())
        body_file.close();
}