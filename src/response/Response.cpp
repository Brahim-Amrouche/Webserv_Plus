/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:49:43 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/06 19:58:34 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Response::ResponseException::ResponseException(const response_err &err, Response *cln)
    :TException("Response Error: ", err, cln)
{
    switch (err)
    {
        case E_FAILED_WRITE:
            this->msg += "Failed to write to socket";
            break;
        default:
            this->msg += "Unknown error";
            break;
    }
}

Response::Response(string &res_id, Socket &c_sock, Request &r):req_id(id)
    ,client_sock(c_sock), req(r)
{}


