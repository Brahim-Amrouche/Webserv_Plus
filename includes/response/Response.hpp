/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:28:49 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/09 15:55:34 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"
#include "Cgi.hpp"

typedef enum RESPONSE_ERR_CODES
{
    E_FAILED_WRITE,
} response_err;




class Response
{
    private:
        char (&res_buf)[HEADERS_MAX_SIZE + 1];
        Request &req;
        Cgi cgi;
        response_code code;
        ssize_t buffer_size;
        string  response_body;
        bool   response_done;
    public:
        class ResponseException : TException<response_err, Response>
        {
            public:
                ResponseException(const response_err &err, Response *cln);
        };
        Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r);
        void pushDefaultHeaders();
        void listDirectory();
        void serveFile(Path &path_dir);
        void redirect(const deque<string> &redi_conf);
        void findResponseFile(const Path &index_page);
        void generateResponse();
        bool operator>>(Socket &clien_sock);
        ~Response();
};