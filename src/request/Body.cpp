/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:23:26 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/05 17:57:02 by bamrouch         ###   ########.fr       */
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
        default :
            msg += "Unknown Error";
    }
}

Body::Body(string &id, char (&buf)[HEADERS_MAX_SIZE + 1], ServerConfiguration *(&c), Headers &h)
    :req_id(id), buffer(buf),conf(c), headers(h) ,mode(M_NO_CONF), body_done(false) 
{
    content_length = -1;   
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
    max_config_size = strtoll(start, &end, 10);
    if (end == start || *end != '\0')
        throw BodyException(E_INVALID_BODY_HEADERS, NULL);
}

void Body::fromHeaders()
{
    if (headers[TRANSFER_ENCODING] != "")
    {
        if (headers[TRANSFER_ENCODING] == "chunked")
            mode = M_CHUNKED;
        else if (headers[TRANSFER_ENCODING] == "identity")
            mode = M_IDENTITY;
        else
            throw BodyException(E_INVALID_BODY_HEADERS, NULL);
    }
    if (headers[CONTENT_LENGTH] != "" && (mode == M_CHUNKED || mode == M_IDENTITY))
        throw BodyException(E_INVALID_BODY_HEADERS, NULL);
    else if (headers[CONTENT_LENGTH] != "")
    {
        const char *start = headers[CONTENT_LENGTH].c_str();
        char *end = NULL;
        content_length = strtoll(start, &end, 10);
        if (end == start || *end != '\0' || content_length > max_config_size)
            throw BodyException(E_INVALID_BODY_HEADERS, NULL);
        mode = M_CONTENT_LENGTH;
    }
    else
        mode = M_NO_BODY;
}

void Body::configBody()
{
    fromConfig();
    fromHeaders();
}

bool Body::readChunked(ssize_t &buffer_size)
{
    // check for bytes count;
    if(body_size + buffer_size > max_config_size)
        throw BodyException(E_BODY_SIZE_OVERFLOW, NULL);
}

bool Body::readIdentity(ssize_t &buffer_size)
{
    if (body_size + buffer_size > max_config_size)
        throw BodyException(E_BODY_SIZE_OVERFLOW, NULL);
    body_file.write(buffer, buffer_size);
    body_size += buffer_size;
    return true;
}

bool Body::readContentLength(ssize_t &buffer_size)
{
    if (body_size + buffer_size > content_length)
        throw BodyException(E_BODY_SIZE_OVERFLOW, NULL);
    body_file.write(buffer, buffer_size);
    body_size += buffer_size;
    return body_size == content_length;
}

bool Body::operator<<(ssize_t &buffer_size)
{
    if (mode == M_NO_CONF)
        configBody();
    body_file.open(req_id.c_str(), std::ios::app | std::ios::binary);
    if (mode == M_NO_BODY)
        body_done = true;
    else if (mode == M_CHUNKED)
        body_done = readChunked(buffer_size);
    else if (mode == M_IDENTITY)
        body_done = readIdentity(buffer_size);
    else if (mode == M_CONTENT_LENGTH)
        body_done = readContentLength(buffer_size);
    body_file.close();
    return body_done;
}


