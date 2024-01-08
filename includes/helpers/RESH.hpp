/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESH.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 20:48:49 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/08 21:13:15 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <exception>
#include "Response.hpp"

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
        static string getStatusLine(const response_code &code);
};