/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PH.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 14:06:45 by bamrouch          #+#    #+#             */
/*   Updated: 2023/12/30 19:05:17 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void PH::validIp(const string &host_str, size_t start, size_t len)
{
    if (len > 3)
        throw PHException();
    string numb = host_str.substr(start, len);
    stringstream ss(numb);
    int num = 0;
    if (!FT::strIsDigit(numb) || !(ss >> num) || num < 0 || num > 255)
        throw PHException();
}

void PH::parseHostString(const string &str, string (&info)[2])
{
    string host, port;
    size_t colons_pos = str.find_first_of(":");
    if (colons_pos != string::npos)
    {
        size_t point1, point2, point3;
        string numb1, numb2, numb3, numb4;
        host = str.substr(0, colons_pos);
        if (colons_pos + 1 < str.size())
            port = str.substr(colons_pos + 1, string::npos);
        else
            throw PHException();
        point1 = host.find_first_of('.');
        if (point1 == string::npos)
            throw PHException();
        point2 = host.find_first_of('.', point1 + 1);
        if (point2 == string::npos)
            throw PHException();
        point3 = host.find_first_of('.', point2 + 1);
        if (point3 == string::npos)
            throw PHException();
        PH::validIp(host, 0, point1);
        PH::validIp(host, point1 + 1, point2 - point1 - 1);
        PH::validIp(host, point2 + 1, point3 - point2 - 1);
        PH::validIp(host, point3 + 1, string::npos);
    }
    else
        port = str;
    stringstream ss(port);
    int port_num = 0;
    if (!FT::strIsDigit(port) || !(ss >> port_num) || port_num < 0)
        throw PHException();
    if (!host.size())
        info[0] = "localhost";
}

string PH::parseBodySize(const string &str)
{
    size_t size_pos = str.find_first_of("MK");
    if (size_pos == string::npos || size_pos + 1 < str.size())
        throw PHException();  
    string body_size = str.substr(0, size_pos);
    stringstream ss(body_size);
    long long bytes_size = 0;
    if (!(ss >> bytes_size) || bytes_size <= 0)
        throw PHException();
    if (str[size_pos] == 'K')
        bytes_size *= 1024;
    else if (str[size_pos] == 'M')
        bytes_size *= 1024 * 1024;
    ostringstream oss;
    oss << bytes_size;
    return oss.str();
}

string PH::strIsAllowedMethod(list<string>::iterator &start, list<string>::iterator &end, deque<string> *methods)
{
    while (start != end && *start != ";")
    {
        if (methods->size() == 4 || (*start != "GET" && *start != "POST" && *start != "DELETE"))
            throw PHException();
        for (size_t i = 1; i < methods->size(); i++)
            if (methods->at(i) == *start)
                throw PHException();
        methods->push_back(*start);
        std::advance(start, 1);
    }
}

http_code_type PH::getHttpCodeType(const string &code)
{
    if (!FT::strIsDigit(code) || code.size() != 3)
        return UNVALID_CODE;
    stringstream ss(code);
    int val = 0;
    if (!(ss >> val) || val < 100 || val > 599)
        return UNVALID_CODE;
    if (val >= 100 && val < 200)
        return INFORMATIONAL_CODE;
    else if (val >= 200 && val < 300)
        return SUCCESSFULL_CODE;
    else if (val >= 300 && val < 400)
        return REDIRECTION_CODE;
    else if (val >= 400 && val < 500)
        return CLIENT_ERROR_CODE;
    else if (code[0] == '5')
        return SERVER_ERROR_CODE;
    else
        return UNVALID_CODE;
}

bool PH::strIsBreakToken(const string &str)
{
    if (str == ";" || str == "{" || str == "}")
        return true;
    return false;
};

bool PH::strIsPath(const string &str)
{
    if (str.size() && str[0] == '/')
        return true;
    return false;
}
