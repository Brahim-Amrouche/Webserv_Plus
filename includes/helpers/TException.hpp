/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TException.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 10:12:59 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/01 21:49:41 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>
#include <string>
#include <iostream>

using std::exception;
using std::string;


template <typename ERR_T, typename CLEANUP_T>
class TException : exception
{
    protected:
        string msg;
        CLEANUP_T *cleanup;
    public :
        ERR_T err_c;
        TException(const string n_msg, const ERR_T &err, CLEANUP_T *cln): msg(n_msg), cleanup(cln), err_c(err)
        {
            if (cleanup)
            {
                delete cleanup;
                cleanup = NULL;
            }
        };
        virtual const char *what() const throw()
        {
            return msg.c_str();
        };
        ERR_T get_err() const throw()
        {
            return err_c;
        };
        virtual ~TException() throw(){};
};