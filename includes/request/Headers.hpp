/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 20:34:53 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/06 17:40:25 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"

#define HEADERS_MAX_SIZE 8192

typedef map<string, string>::iterator HeadersIt;

typedef enum E_HEADERS_ERR_CODES
{
    E_HEADERS_OVERFLOW,
    E_DUPLICATE_HEADERS,
    E_REQUEST_LINE,
    E_INVALID_METHOD,
    E_INVALID_HTTP_VERSION,
    E_NO_HOST_HEADER
} headers_err;

enum REQUEST_HEADERS
{
    HOST,
    CONTENT_LENGTH,
    TRANSFER_ENCODING,
};

extern const char *request_headers[3];

class Headers
{
    private:
        char  (&req_buffer)[HEADERS_MAX_SIZE + 1];
        ServerSocket &server_sock;
        ServerConfiguration *(&req_config);
        map<string, string> headers;
        string  req_line;
        string  req_path;
        bool    headers_done;
    public:
        class HeadersException: public TException< headers_err, Headers>
        {
            public:
                HeadersException(const headers_err &err, Headers *cln); 
        };
        Headers(char (&buffer)[HEADERS_MAX_SIZE + 1], ServerSocket &srv_sock, ServerConfiguration *(&conf));
        void parseRequestLine();
        void configureRequest();
        string operator[](const REQUEST_HEADERS &key);
        bool   operator<<(ssize_t &buffer_size);
        bool   headersDone();
        void   debugHeaders();
        ~Headers();
};
