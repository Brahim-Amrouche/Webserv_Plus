/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TException.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 10:12:59 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/25 14:02:51 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>
#include <string>

using std::exception;
using std::string;

template <typename ERR_T, typename CLEANUP_T>
class TException : exception
{
    protected:
        string msg;
        ERR_T err_c;
        CLEANUP_T *cleanup;
    public :
        TException(const string n_msg, const ERR_T &err, const CLEANUP_T *cln): msg(n_msg), err_c(err), cleanup(cln)
        {
            if (cleanup)
                delete cleanup;
        };
        const char *what() const throw()
        {
            return msg.c_str();
        };
        ERR_T get_err() const throw()
        {
            return err_c;
        };
        virtual ~TException() throw(){};
};