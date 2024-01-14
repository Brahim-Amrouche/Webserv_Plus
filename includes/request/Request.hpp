/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:23:31 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/14 18:48:16 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"
#include "ServerConfiguration.hpp"
#include "TException.hpp"
#include "Headers.hpp"
#include "Body.hpp"

class Client;

typedef enum REQUEST_ERR_CODES
{
    E_FAILED_READ,
    E_FAILED_HEADERS_READ,
    E_FAILED_BODY_READ,
    E_REQUEST_ERROR_400,
    E_REQUEST_ERROR_405,
    E_REQUEST_ERROR_500,
    E_READING_DONE,
} request_err;

class Request
{
    private:
        string  req_id;
        char  (&req_buffer)[HEADERS_MAX_SIZE + 1];
        ssize_t   buffer_size;
        Socket  &client_sock;
        ServerSocket &server_sock;
        Client &client;
        ServerConfiguration *server_config;
        Headers headers;
        Body body;
    public:
        class RequestException : public TException<request_err, Request>
        {
            public:
                RequestException(const request_err &err, Request *cln);
        };
        Request(char (&buffer)[HEADERS_MAX_SIZE + 1], Socket &sock_id, ServerSocket &sock, Client &cl);
        void read();
        string &getReqPath()
        {
            return (headers.getReqPath());
        };
        REQUEST_METHOD getReqMethod()
        {
            return (headers.getReqMethod());
        };
        string &getReqId()
        {
            return (req_id);
        };
        BODY_MODE getBodyMode()
        {
            return (body.getMode());
        };
        bool getBodyDone()
        {
            return (body.getBodyDone());
        };
        ssize_t getBodySize()
        {
            return (body.getBodySize());
        };
        ServerConfiguration *getServerConfig()
        {
            return (server_config);
        };
        Socket &getClientSock()
        {
            return (client_sock);
        };
        ServerSocket &getServerSock()
        {
            return (server_sock);
        };
        string &operator*();
        deque<string> *operator[](const string &server_key);
        string         operator[](const REQUEST_HEADERS &key);
        ~Request();
        
    
};