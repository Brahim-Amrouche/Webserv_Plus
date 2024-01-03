/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   REQH.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:29:17 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/03 13:57:50 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


ssize_t REQH::get_headers(const string &req, const ssize_t &headers_size, string &req_path, map<string,string> &req_headers)
{
    if (req.find("\r\n\r\n") == string::npos)
        return -1;
    stringstream ss(req);
    string line;
    std::getline(ss, req_path);
    ssize_t pos = req_path.size() + 1;
    while (std::getline(ss, line) && pos < headers_size)
    {
        pos += line.size() + 1;   
        if (line == "\r")
            break;
        size_t colon_pos = line.find(": ");
        if (colon_pos != string::npos)
        {
            string key = line.substr(0, colon_pos);
            string value = line.substr(colon_pos + 2, line.size() - 1);
            if (req_headers.find(key) != req_headers.end())
                throw REQHException();    
            req_headers.insert(std::pair<string, string>(key, value));   
        }
    }
    return pos;
}

string REQH::generateReqId()
{

    static const char* hexChars = "0123456789ABCDEF";
    const int uuidLength = 36;

    std::srand(std::time(0));
    stringstream ss;
    ss << "./tmp/";
    for (int i = 0; i < uuidLength; ++i) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            ss << '-';
        } else {
            ss << hexChars[std::rand() % 16];
        }
    }
    ss << ".tmp";
    string request_id = ss.str();
    size_t last_dot = request_id.find_last_of('.');
    stringstream num;
    for (size_t i = 0; access(request_id.c_str(), F_OK) == 0; i++)
    {
        request_id = ss.str();
        num << i;
        request_id.insert(last_dot, num.str());
        num.str("");
    }
    return request_id;
}