/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   REQH.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:05:47 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/02 22:53:46 by bamrouch         ###   ########.fr       */
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
        static ssize_t get_headers(const string &buffer,const ssize_t &headers_size, map<string, string> &req_headers);
        static string  generateReqId();
};
