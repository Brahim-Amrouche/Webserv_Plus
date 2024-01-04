/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:23:26 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/05 00:48:42 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

Body::Body(char (&buffer)[HEADERS_MAX_SIZE + 1], string &req_id)
{}

Body::Body(string &req_id, ssize_t &read_bytes)
{
    
}

void Body::configBody(Headers &headers)
{
    if (headers["Transfer-Encoding"] == "chunked")
        mode = CHUNKED;
    else if (headers["Content-Length"] != "")
    {
        mode = CONTENT_LENGTH;
        // parse for Content-Length issues;
        body_size = stoi(headers["Content-Length"]);
    }
    else
        mode = NO_BODY;
}


