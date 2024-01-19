
#pragma once
#include "../server/Socket.hpp"
#include "Request.hpp"

#define HEADERS_MAX_SIZE 32768

typedef map<string, string>::iterator HeadersIt;

typedef enum E_HEADERS_ERR_CODES
{
    E_HEADERS_OVERFLOW,
    E_DUPLICATE_HEADERS,
    E_REQUEST_LINE,
    E_INVALID_METHOD,
    E_INVALID_HTTP_VERSION,
    E_NO_HOST_HEADER,
    E_INVALID_CONTENT_TYPE,
} headers_err;

enum REQUEST_HEADERS
{
    HOST,
    CONTENT_LENGTH,
    TRANSFER_ENCODING,
    CONTENT_TYPE,
    UPLOAD_FILE,
    COOKIE,
};

extern const char *request_headers[6];

class ServerSocket;
class Socket;

class Headers
{
    private:
        char  (&req_buffer)[HEADERS_MAX_SIZE + 1];
        ServerSocket &server_sock;
        ServerConfiguration *&req_config;
        map<string, string> headers;
        string  req_line;
        string  req_path;
        REQUEST_METHOD req_method;
        bool    headers_done;
    public:
        class HeadersException: public TException< headers_err, Headers>
        {
            public:
                HeadersException(const headers_err &err, Headers *cln); 
        };
        Headers(char (&buffer)[HEADERS_MAX_SIZE + 1], ServerSocket &srv_sock, ServerConfiguration *(&conf));
        void parseRequestLine();
        void configureRequest();
        string &getReqPath()
        {
            return (req_path);
        };
        REQUEST_METHOD getReqMethod()
        {
            return (req_method);
        };
        string operator[](const REQUEST_HEADERS &key);
        bool   operator<<(ssize_t &buffer_size);
        bool   headersDone();
        void   debugHeaders();
        ~Headers();
};

