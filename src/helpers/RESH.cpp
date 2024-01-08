/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESH.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 20:54:03 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/08 21:15:54 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

string RESH::getStatusLine(const response_code &code)
{
    ostringstream oss;
    oss << "HTTP/1.1 " << code << " " << response_handle[code] << "\r\n";
    return (string(oss.str()));
}
