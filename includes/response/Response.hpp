/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:28:49 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/06 19:58:22 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"

typedef enum RESPONSE_ERR_CODES
{
    E_FAILED_WRITE,
} response_err;



class Response
{
    private:
        string &req_id;
        Socket &client_sock;
        Request &req;
    public:
        class ResponseException : TException<response_err, Response>
        {
            public:
                ResponseException(const response_err &err, Response *cln);
        };
        Response(string &id, Socket &c_sock, Request &r);
        ~Response();
};