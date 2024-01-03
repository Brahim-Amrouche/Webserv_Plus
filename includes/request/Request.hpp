/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:23:31 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/03 21:12:35 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../server/Socket.hpp"
#include "../config/ServerConfiguration.hpp"
#include "../helpers/TException.hpp"
#include <fstream>
#define REQ_BUFFER_SIZE 8192

using std::ofstream;

typedef enum REQUEST_ERR_CODES
{
    E_FAILED_READ,
    E_REQ_BUFFER_OVERFLOW,
    E_DUPLICATE_HEADERS,
    E_REQUEST_LINE,
    E_NO_HOST_HEADER,
    E_FAILED_BODY_WRITE,
    E_BODY_SIZE_OVERFLOW,
    E_READING_DONE,
} request_err;

typedef map<string, string>::iterator HeadersIt;

enum REQUEST_HEADERS
{
    HOST,
    CONTENT_LENGTH,
    TRANSFER_ENCODING,
} ;

extern const char *request_headers[3];


class Request
{
    private:
        char      req_buffer[REQ_BUFFER_SIZE + 1];
        string    req_method;
        map<string, string> req_headers;
        Socket  &client_sock;
        ServerSocket &server_sock;
        ssize_t   headers_size;
        ssize_t   counter;
        ssize_t   body_size;
        ServerConfiguration *server_config;
        string  req_id;
        bool    headers_done;
        bool    body_done;
    public:
        map<string, string>::iterator Headersit;
        class RequestException : public TException<request_err, Request>
        {
            public:
                RequestException(const request_err &err, Request *cln);
        };
        Request();
        Request(Socket &sock_id, ServerSocket &sock);
        Request(const Request &cpy_req);
        Request &operator=(const Request &eq_req);
        void    operator<<(const char *buffer);
        void parseRequestLine();
        void configureRequest();
        void readHeaders();
        void readBody();
        void read();
        void debugHeaders();
        string operator[](const REQUEST_HEADERS &key);
        ~Request();
        
    
};