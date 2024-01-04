/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 20:21:15 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/05 00:50:14 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"

typedef enum E_BODY_ERR_CODES
{
    E_BODY_OVERFLOW,
    E_BODY_READING,
} body_err;

enum BODY_MODE
{
    CHUNKED,
    CONTENT_LENGTH,
    NO_BODY,
};

class Body
{
    private:
        char   (&buffer)[HEADERS_MAX_SIZE + 1];
        string  &req_id;
        ssize_t max_config_size;
        ssize_t content_length;
        ssize_t body_size;
        BODY_MODE mode;
        bool    body_done;
    public:
        class BodyException: public TException< body_err, Body>
        {
            public:
                BodyException(const body_err &err, Body *cln); 
        };
        Body(char (&buffer)[HEADERS_MAX_SIZE + 1], string &req_id);
        Body(string &req_id, ssize_t &read_bytes);
        void configBody(Headers &headers);
        bool operator<<(ssize_t &buffer_size);
        ~Body();
};