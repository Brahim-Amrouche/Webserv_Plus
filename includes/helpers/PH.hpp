/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PH.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 13:54:13 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/30 19:02:43 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <sstream>
#include <exception>
#include <iostream>
#include <list>

using std::string;
using std::stringstream;
using std::exception;
using std::ostringstream;
using std::list;

typedef enum HTTP_CODE_TYPE
{
    UNVALID_CODE = -1,
    INFORMATIONAL_CODE,
    SUCCESSFULL_CODE,
    REDIRECTION_CODE,
    CLIENT_ERROR_CODE,
    SERVER_ERROR_CODE
} http_code_type;

class PH
{
    public:
        class PHException : public exception
        {
            public:
                const char *what() const throw()
                {
                    return "PHException";
                };
        };
        static void parseHostString(const string &str, string (&info)[2]);
        static void validIp(const string &host_str, size_t start, size_t len);
        static string parseBodySize(const string &str);
        static string strIsAllowedMethod(list<string>::iterator &start, list<string>::iterator &end, deque<string> *methods);
        static http_code_type getHttpCodeType(const string &code);
        static bool strIsBreakToken(const string &str);
        static bool strIsPath(const string &str);
};

