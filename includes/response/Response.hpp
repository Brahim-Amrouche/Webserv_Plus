/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:28:49 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/07 17:22:21 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"
#include "Cgi.hpp"

typedef enum RESPONSE_ERR_CODES
{
    E_FAILED_WRITE,
} response_err;

typedef enum RESPONSE_CODE
{
    NONE = 0,
    RES_OK = 200,
    RES_BAD_REQUEST = 400,
    RES_FORBIDDEN = 403,
    RES_INTERNAL_SERVER_ERROR = 500,
    RES_NOT_IMPLEMENTED = 501,
    RES_SERVICE_UNAVAILABLE = 503,
} response_code;


class Response
{
    private:
        char (&res_buf)[HEADERS_MAX_SIZE + 1];
        Request &req;
        response_code code;
        ssize_t buffer_size;
        string  response_path;
    public:
        class ResponseException : TException<response_err, Response>
        {
            public:
                ResponseException(const response_err &err, Response *cln);
        };
        Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r);
        void listDirectory();
        void findResponseFile(const Path &index_page);
        void generateResponse();
        bool operator>>(Socket &clien_sock);
        ~Response();
};