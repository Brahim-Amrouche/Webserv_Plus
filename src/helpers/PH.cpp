
#include "webserv.hpp"

void PH::validIp(const string &host_str, size_t start, size_t len)
{
    string numb = host_str.substr(start, len);
    if (numb.size() == 0 || numb.size() > 3)
        throw PHException();
    stringstream ss(numb);
    int num = 0;
    if (!FT::strIsDigit(numb) || !(ss >> num) || num < 0 || num > 255)
        throw PHException();
}

void PH::getHostString(const string &str, string &host, string &port, size_t colons_pos)
{
    size_t point1, point2, point3;
    string numb1, numb2, numb3, numb4;
    host = str.substr(0, colons_pos);
    if (colons_pos + 1 < str.size())
        port = str.substr(colons_pos + 1, string::npos);
    else
        throw PHException();
    host = FT::strToLowercase(host);
    if (host == "localhost")
        return;
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

void PH::parseHostString(const string &str, string (&info)[2])
{
    string host, port;
    size_t colons_pos = str.find_first_of(":");
    if (colons_pos != string::npos)
        PH::getHostString(str, host, port, colons_pos);
    else
        port = str;
    stringstream ss(port);
    int port_num = 0;
    if (!FT::strIsDigit(port) || !(ss >> port_num) || port_num < 0)
        throw PHException();
    if (!host.size())
        host = "localhost";
    info[0] = host;
    info[1] = port;
}

string PH::parseBodySize(const string &str)
{
    size_t size_pos = str.find_first_of("MK");
    if (size_pos != string::npos && size_pos + 1 < str.size())
        throw PHException();  
    string body_size = str.substr(0, size_pos);
    stringstream ss(body_size);
    long long bytes_size = 0;
    if (!(ss >> bytes_size) || bytes_size <= 0)
        throw PHException();
    if (size_pos == string::npos)
        ;
    else if (str[size_pos] == 'K')
        bytes_size *= 1024;
    else if (str[size_pos] == 'M')
        bytes_size *= 1024 * 1024;
    ostringstream oss;
    oss << bytes_size;
    return oss.str();
}

void PH::strIsAllowedMethod(list<string>::iterator &start, list<string>::iterator &end, deque<string> *methods)
{
    bool method_set = false;
    while (start != end && *start != ";")
    {
        if (methods->size() == 4 || (*start != "GET" && *start != "POST" && *start != "DELETE"))
            throw PHException();
        for (size_t i = 1; i < methods->size(); i++)
            if (methods->at(i) == *start)
                throw PHException();
        method_set = true;
        methods->push_back(*start);
        std::advance(start, 1);
    }
    if (start == end || !method_set || *start != ";")
        throw PHException();
}

response_code PH::isInformational_code(int &code)
{
    if (code < 1  || code > 99)
        return RES_NONE;
    switch(code)
    {
        case 100:
        case 101:
        case 102:
        case 103:
            return static_cast<response_code>(code);
        default:
            return RES_NONE;
    }
}

response_code PH::isSuccessful_code(int &code)
{
    if (code < 100  || code > 299)
        return RES_NONE;
    switch (code)
    {
        case 200:
        case 201:
        case 202:
        case 203:
        case 204:
        case 205:
        case 206:
        case 207:
        case 226:
            return static_cast<response_code>(code);
        default:
            return RES_NONE;
    }
}

response_code PH::isRedirection_code(int &code)
{
    if (code < 300  || code > 399)
        return RES_NONE;
    switch (code)
    {
        case 300:
        case 301:
        case 302:
        case 303:
        case 304:
        case 305:
        case 307:
        case 308:
            return static_cast<response_code>(code);
        default:
            return RES_NONE;
    }
}

response_code PH::isClientError_code(int &code)
{
    if (code < 400  || code > 499)
        return RES_NONE;
    switch(code)
    {
        case 400:
        case 401:
        case 402:
        case 403:
        case 404:
        case 405:
        case 406:
        case 407:
        case 408:
        case 409:
        case 410:
        case 411:
        case 412:
        case 413:
        case 414:
        case 415:
        case 416:
        case 417:
        case 418:
        case 422:
        case 423:
        case 424:
        case 426:
        case 428:
        case 429:
        case 431:
        case 451:
            return static_cast<response_code>(code);
        default:
            return RES_NONE;
    }
}

response_code PH::isServerError_code(int &code)
{
    if (code < 500  || code > 599)
        return RES_NONE;
    switch(code)
    {
        case 500:
        case 501:
        case 502:
        case 503:
        case 504:
        case 505:
        case 506:
        case 507:
        case 508:
        case 510:
        case 511:
            return static_cast<response_code>(code);
        default:
            return RES_NONE;
    }
}

response_code PH::getHttpCode(const string &code)
{
    response_code res = RES_NONE;
    if (!FT::strIsDigit(code) || code.size() != 3)
        return res;
    stringstream ss(code);
    int val = 0;
    if (!(ss >> val) || val < 100 || val > 599)
        return res;
    if ((res = isInformational_code(val)))
        return res;
    else if ((res = isSuccessful_code(val)))
        return res;
    else if ((res = isRedirection_code(val)))
        return res;
    else if ((res = isClientError_code(val)))
        return res;
    else if ((res = isServerError_code(val)))
        return res;
    else
        return res;
}

http_code_type PH::getHttpCodeType(const string &code)
{
    if (!FT::strIsDigit(code) || code.size() != 3)
        return UNVALID_CODE;
    stringstream ss(code);
    int val = 0;
    if (!(ss >> val) || val < 100 || val > 599)
        return UNVALID_CODE;
    if (isInformational_code(val) != RES_NONE)
        return INFORMATIONAL_CODE;
    else if (isSuccessful_code(val) != RES_NONE)
        return SUCCESSFULL_CODE;
    else if (isRedirection_code(val) != RES_NONE)
        return REDIRECTION_CODE;
    else if (isClientError_code(val) != RES_NONE)
        return CLIENT_ERROR_CODE;
    else if (isServerError_code(val) != RES_NONE)
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

bool PH::strIsPath(string &str)
{
    if (str[0] != '/')
        return false;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '/' && i + 1 < str.size() && (str[i + 1] == '/' || str[i + 1] == '.' ))
            return false;
    }
    FT::discardLastSlash(str);
    return true;
}

bool PH::strIsPath(const string &str)
{
    if (str[0] != '/')
        return false;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '/' && i + 1 < str.size() && (str[i + 1] == '/' || str[i + 1] == '.') )
            return false;
    }
    return true;
}


void PH::parseErrorPage(list<string>::iterator &start, list<string>::iterator &end, deque<string> *error_list)
{
    size_t error_codes = 0;
    bool path_set = false;
    while (start != end && *start != ";")
    {
        if (PH::strIsPath(*start))
        {
            if (error_codes == 0 || std::find(error_list->begin(), error_list->end(), *start) != error_list->end())
                throw PHException();
            error_list->push_back(*start);
            std::advance(start, 1);
            path_set = true;
            break;
        }
        else if (std::find(error_list->begin(), error_list->end(), *start) != error_list->end() || PH::getHttpCodeType(*start) == UNVALID_CODE 
            || (PH::getHttpCodeType(*start) != CLIENT_ERROR_CODE && PH::getHttpCodeType(*start) != SERVER_ERROR_CODE) )
            throw PHException();
        error_list->push_back(*start);
        ++error_codes;
        std::advance(start, 1);
    }
    if (error_codes == 0 || !path_set || *start != ";")
        throw PHException();
}

// void PH::parseRedirection(list<string>::iterator &start, list<string>::iterator &end, deque<string> *redir_list)
// {
//     if (start == end || PH::getHttpCodeType(*start) == UNVALID_CODE || PH::getHttpCodeType(*start) != REDIRECTION_CODE)
//         throw PHException();
//     redir_list->push_back(*start);
//     std::advance(start, 1);
//     if (start == end || !PH::strIsPath(*start))
//         throw PHException();
//     redir_list->push_back(*start);
//     std::advance(start, 1);
//     if  (start == end || *start != ";")
//         throw PHException();
// }