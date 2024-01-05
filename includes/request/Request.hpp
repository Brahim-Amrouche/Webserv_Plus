/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:23:31 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/05 22:43:38 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../server/Socket.hpp"
#include "../config/ServerConfiguration.hpp"
#include "../helpers/TException.hpp"
#include "Headers.hpp"
#include "Body.hpp"

typedef enum REQUEST_ERR_CODES
{
    E_FAILED_READ,
    E_FAILED_HEADERS_READ,
    E_FAILED_BODY_READ,
    E_READING_DONE,
} request_err;

class Request
{
    private:
        string  req_id;
        char      req_buffer[HEADERS_MAX_SIZE + 1];
        ssize_t   buffer_size;
        Socket  &client_sock;
        ServerSocket &server_sock;
        ServerConfiguration *server_config;
        Headers headers;
        Body body;
    public:
        class RequestException : public TException<request_err, Request>
        {
            public:
                RequestException(const request_err &err, Request *cln);
        };
        Request();
        Request(Socket &sock_id, ServerSocket &sock);
        Request(const Request &cpy_req);
        Request &operator=(const Request &eq_req);
        void read();
        string operator[](const REQUEST_HEADERS &key);
        ~Request();
        
    
};