
#pragma once
#include <string>
#include <sstream>
#include <exception>
#include <iostream>
#include <list>
#include <deque>

using std::string;
using std::stringstream;
using std::exception;
using std::ostringstream;
using std::list;
using std::deque;

typedef enum RESPONSE_CODES
{
    RES_NONE = 0,
    RES_CONTINUE = 100,
    RES_SWITCHING_PROTOCOLS = 101,
    RES_PROCESSING = 102,
    RES_EARLY_HINTS = 103,
    RES_OK = 200,
    RES_CREATED = 201,
    RES_ACCEPTED = 202,
    RES_NON_AUTHORITATIVE_INFORMATION = 203,
    RES_NO_CONTENT = 204,
    RES_RESET_CONTENT = 205,
    RES_PARTIAL_CONTENT = 206,
    RES_MULTI_STATUS = 207,
    RES_ALREADY_REPORTED = 208,
    RES_IM_USED = 226,
    RES_MULTIPLE_CHOICES = 300,
    RES_MOVE_PERMANENTLY = 301,
    RES_FOUND = 302,
    RES_SEE_OTHER = 303,
    RES_NOT_MODIFIED = 304,
    RES_USE_PROXY = 305,
    RES_SWITCH_PROXY = 306,
    RES_TEMPORARY_REDIRECT = 307,
    RES_PERMANENT_REDIRECT = 308,
    RES_BAD_REQUEST = 400,
    RES_UNAUTHORIZED = 401,
    RES_PAYMENT_REQUIRED = 402,
    RES_FORBIDDEN = 403,
    RES_NOT_FOUND = 404,
    RES_METHOD_NOT_ALLOWED = 405,
    RES_NOT_ACCEPTABLE = 406,
    RES_PROXY_AUTHENTICATION_REQUIRED = 407,
    RES_REQUEST_TIMEOUT = 408,
    RES_CONFLICT = 409,
    RES_GONE = 410,
    RES_LENGTH_REQUIRED = 411,
    RES_PRECONDITION_FAILED = 412,
    RES_PAYLOAD_TOO_LARGE = 413,
    RES_URI_TOO_LONG = 414,
    RES_UNSUPPORTED_MEDIA_TYPE = 415,
    RES_RANGE_NOT_SATISFIABLE = 416,
    RES_EXPECTATION_FAILED = 417,
    RES_IM_A_TEAPOT = 418,
    RES_MISDIRECTED_REQUEST = 421,
    RES_UNPROCESSABLE_ENTITY = 422,
    RES_LOCKED = 423,
    RES_FAILED_DEPENDENCY = 424,
    RES_UPGRADE_REQUIRED = 426,
    RES_PRECONDITION_REQUIRED = 428,
    RES_TOO_MANY_REQUESTS = 429,
    RES_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    RES_UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    RES_INTERNAL_SERVER_ERROR = 500,
    RES_NOT_IMPLEMENTED = 501,
    RES_BAD_GATEWAY = 502,
    RES_SERVICE_UNAVAILABLE = 503,
    RES_GATEWAY_TIMEOUT = 504,
    RES_HTTP_VERSION_NOT_SUPPORTED = 505,
    RES_VARIANT_ALSO_NEGOTIATES = 506,
    RES_INSUFFICIENT_STORAGE = 507,
    RES_LOOP_DETECTED = 508,
    RES_NOT_EXTENDED = 510,
    RES_NETWORK_AUTHENTICATION_REQUIRED = 511,
} response_code;

typedef enum HTTP_CODE_TYPE
{
    UNVALID_CODE = -1,
    INFORMATIONAL_CODE,
    SUCCESSFULL_CODE,
    REDIRECTION_CODE,
    CLIENT_ERROR_CODE,
    SERVER_ERROR_CODE
} http_code_type;

class PH
{
    public:
        class PHException : public exception
        {
            public:
                const char *what() const throw()
                {
                    return "PHException";
                };
        };
        static void parseHostString(const string &str, string (&info)[2]);
        static void validIp(const string &host_str, size_t start, size_t len);
        static void strIsAllowedMethod(list<string>::iterator &start, list<string>::iterator &end, deque<string> *methods);
        static string parseBodySize(const string &str);
        static response_code isInformational_code(int &code);
        static response_code isSuccessful_code(int &code);
        static response_code isRedirection_code(int &code);
        static response_code isClientError_code(int &code);
        static response_code isServerError_code(int &code);
        static response_code getHttpCode(const string &code);
        static http_code_type getHttpCodeType(const string &code);
        static bool strIsBreakToken(const string &str);
        static bool strIsPath(string &str);
        static bool strIsPath(const string &str);
        static void parseErrorPage(list<string>::iterator &start, list<string>::iterator &end, deque<string> *error_list);
        static void getHostString(const string &str, string &host, string &port, size_t colons_pos);
        // static void parseRedirection(list<string>::iterator &start, list<string>::iterator &end, deque<string> *redirection_list);
};

