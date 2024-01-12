/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:28:49 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/12 18:05:04 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/types.h>
#include <dirent.h>
#include "Request.hpp"
#include "Cgi.hpp"
#include "File.hpp"

typedef enum RESPONSE_ERR_CODES
{
    E_FAILED_SEND,
    E_FAILED_RESPONSE_BODY_READ,
    E_FAILED_UPLOADING_FILE,
    E_FAILED_CGI_EXEC,
    E_CLOSE_CONNECTION,
} response_err;

class Response
{
    private:
        char (&res_buf)[HEADERS_MAX_SIZE + 1];
        Request &req;
        string root_directory;
        Cgi cgi;
        ssize_t buffer_size;
        bool   res_headers_done;
        response_code error_served;
        File file;
    public:
        class ResponseException : public TException<response_err, Response>
        {
            public:
                ResponseException(const response_err &err, Response *cln);
        };
        Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r);
        void pushDefaultHeaders();
        
        void listDirectory(Path &path_dir);
        void uploadFile();
        void serveErrorHeaders(const response_code &err_code);
        void redirect(Path &redi_conf, const response_code &code);
        void serveFile(Path &path_dir, const response_code &res_code);
        void serveDirectory(Path &path_dir);
        void serveError(const response_code &err_code);

        void generateResponse();
        void operator>>(Socket &clien_sock);
        ~Response();
};