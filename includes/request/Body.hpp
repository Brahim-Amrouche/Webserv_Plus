/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 20:21:15 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/04 23:06:45 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"

class Body
{
    private:
        char   *(&buffer);
        string  &req_id;
        ssize_t &read_bytes;
        ssize_t body_size;
        bool    body_done;
    public:
        Body(string &req_id);
        Body(string &req_id, ssize_t &read_bytes);
        ~Body();
};