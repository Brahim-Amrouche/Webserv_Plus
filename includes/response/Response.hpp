/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:28:49 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/10 15:28:48 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"
#include "Cgi.hpp"
#include "File.hpp"

typedef enum RESPONSE_ERR_CODES
{
    E_FAILED_SEND,
    E_FAILED_RESPONSE_BODY_READ,
    E_CLOSE_CONNECTION,
} response_err;

class Response
{
    private:
        char (&res_buf)[HEADERS_MAX_SIZE + 1];
        Request &req;
        Cgi cgi;
        response_code code;
        ssize_t buffer_size;
        ssize_t res_header_left;
        bool   res_headers_done;
        File file;
    public:
        class ResponseException : public TException<response_err, Response>
        {
            public:
                ResponseException(const response_err &err, Response *cln);
        };
        Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r);
        void pushDefaultHeaders();
        
        void redirect(const deque<string> &redi_conf);
        void serveFile(Path &path_dir);
        void serveDirectory(Path &path_dir);
        
        void findResponseFile(const Path &index_page);
        void generateResponse();
        void operator>>(Socket &clien_sock);
        ~Response();
};