/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESH.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 20:48:49 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/12 15:37:48 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <exception>
#include "Response.hpp"
#include <ctime>
// #include "Headers.hpp"

using std::string;
using std::exception;

class RESH
{
    public:
        class RESHException : public exception
        {
            public:
                virtual const char* what() const throw()
                {
                    return ("RESH Exception");
                }
        };
        static string getMethodString(const REQUEST_METHOD &method);
        static void   pushHeaders(char (&buffer)[HEADERS_MAX_SIZE + 1],const string &header, ssize_t &buffer_size);
        static string getHtmlListTag(const string &path, const string &name);
        static string getStatusLine(const response_code &code);
        static string getStatusRedi(const response_code &code);
        static string getCloseConnectionHeader();
        static string getDateHeader();
        static string getContentLengthHeader(Path &path, ssize_t &res_body_size);
        static string getContentTypeHeader(Path &path);
};