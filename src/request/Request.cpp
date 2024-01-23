
#include "webserv.hpp"

Request::RequestException::RequestException(const request_err &err, Request *cln): TException("Request Error: ", err, cln) 
{
    switch (err_c)
    {
        case E_FAILED_READ:
            msg += "Failed To Read";
            break;
        case E_FAILED_HEADERS_READ:
            msg += "Failed To Read Headers";
            break;
        case E_FAILED_BODY_READ:
            msg += "Failed To Write Body";
            break;
        case E_READING_DONE:
            msg += "Reading Done";
            break;
        default:
            msg += "Unknown Error";
    }
}

Request::Request(char (&buffer)[HEADERS_MAX_SIZE + 1], Socket &c_sock, ServerSocket &s_sock, Client &cl):req_id(REQH::generateReqId()), req_buffer(buffer)
    , buffer_size(0), client_sock(c_sock), server_sock(s_sock), client(cl) ,server_config(NULL)
    , headers(req_buffer, server_sock, server_config), body(req_id, req_buffer, server_config, headers)
{}

void Request::read()
{
    if (headers.headersDone() && body.bodyDone())
        throw RequestException(E_READING_DONE, NULL);
    ssize_t temp = 0;
    if ((temp = recv(client_sock.getSockid(), req_buffer + buffer_size, HEADERS_MAX_SIZE - buffer_size, 0)) <= 0)
        throw RequestException(E_FAILED_READ, NULL);
    client.setLastActivity();
    buffer_size += temp;
    req_buffer[buffer_size] = '\0';
    try
    {
        if ((headers << buffer_size) && (body << buffer_size))
            return;
    }
    catch (const Headers::HeadersException &e)
    {
        switch (e.err_c)
        {
            case E_HEADERS_OVERFLOW:
            case E_DUPLICATE_HEADERS:
            case E_NO_HOST_HEADER:
            case E_INVALID_HTTP_VERSION:
            case E_REQUEST_LINE:
               throw RequestException(E_REQUEST_ERROR_400, NULL);
            case E_INVALID_METHOD:
                throw RequestException(E_REQUEST_ERROR_405, NULL);    
            default:
                throw RequestException(E_REQUEST_ERROR_400, NULL);
        }
    }
    catch (const Body::BodyException &e)
    {
        switch (e.err_c)
        {
            case E_INVALID_BODY_CONFIG:
            case E_INVALID_BODY_HEADERS:
            case E_BODY_SIZE_OVERFLOW:
                throw RequestException(E_REQUEST_ERROR_400, NULL);
            case E_BODY_READING:
            case E_UNABLE_TO_OPEN_TMP_FILE:
                throw RequestException(E_REQUEST_ERROR_500, NULL);
            default:
                throw RequestException(E_REQUEST_ERROR_400, NULL);
        }
    }
}

string &Request::operator*()
{
    deque<string> &location_name = ***server_config;
    return location_name[1];
}

deque<string> *Request::operator[](const string &server_key)
{
    if (!server_config)
        return NULL;
    ServerConfiguration *conf = (*server_config)[server_key];
    if (!conf)
        return NULL;
    return **conf;
}

string Request::operator[](const REQUEST_HEADERS &key)
{
    return headers[key];
}

Request::~Request()
{
    remove(req_id.c_str());
}