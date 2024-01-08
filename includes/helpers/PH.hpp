/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PH.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 13:54:13 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/08 21:19:50 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <sstream>
#include <exception>
#include <iostream>
#include <list>
#include <deque>

using std::string;
using std::stringstream;
using std::exception;
using std::ostringstream;
using std::list;
using std::deque;


#define SUCCESSFULL_CODE(CODE) (
    switch(CODE)
    {
        case 200:
        case 201:
        case 202:
        case 203:
        case 204:
        case 205:
        case 206:
            return (true);
        default:
            return (false);
    }
)

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
        static void strIsAllowedMethod(list<string>::iterator &start, list<string>::iterator &end, deque<string> *methods);
        static string parseBodySize(const string &str);
        static http_code_type getHttpCodeType(const string &code);
        static bool strIsBreakToken(const string &str);
        static bool strIsPath(string &str);
        static bool strIsPath(const string &str);
        static void parseErrorPage(list<string>::iterator &start, list<string>::iterator &end, deque<string> *error_list);
        static void getHostString(const string &str, string &host, string &port, size_t colons_pos);
        // static void parseRedirection(list<string>::iterator &start, list<string>::iterator &end, deque<string> *redirection_list);
};

