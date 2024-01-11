/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   REQH.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:05:47 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/11 18:00:38 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <map>
#include <ctime>

using std::stringstream;
using std::string;
using std::map;

enum REQUEST_METHOD
{
    METHOD_NONE,
    METHOD_GET,
    METHOD_POST,
    METHOD_DELETE,
};


class REQH
{
    public:
        class REQHException : public exception
        {
            public:
                const char *what() const throw()
                {
                    return "REQHException";
                };
        };
        static ssize_t get_headers(const string &buffer,const ssize_t &headers_size, string &req_path, map<string, string> &req_headers);
        static string  generateReqId();
        static REQUEST_METHOD getReqMethod(const string &method);
        static bool    CRLF_found(char *buffer, ssize_t &buffer_size, ssize_t *content_length);
};
