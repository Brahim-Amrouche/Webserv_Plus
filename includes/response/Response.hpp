
#pragma once
#include <sys/types.h>
#include <dirent.h>
#include "Request.hpp"
#include "Cgi.hpp"
#include "File.hpp"

class Client;

typedef enum RESPONSE_ERR_CODES
{
    E_FAILED_SEND,
    E_FAILED_RESPONSE_BODY_READ,
    E_FAILED_UPLOADING_FILE,
    E_FAILED_CGI_EXEC,
    E_CLOSE_CONNECTION,
} response_err;

class Response
{
    private:
        char (&res_buf)[HEADERS_MAX_SIZE + 1];
        Request &req;
        Client &client;
        ssize_t buffer_size;
        File file;
        Cgi cgi;
        bool   res_headers_done;
        response_code error_served;
        string root_directory;
    public:
        class ResponseException : public TException<response_err, Response>
        {
            public:
                ResponseException(const response_err &err, Response *cln);
        };
        Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r, Client &cl);
        void pushDefaultHeaders();
        
        void listDirectory(Path &path_dir);
        void uploadFile();
        void serveErrorHeaders(const response_code &err_code);
        void redirect(Path &redi_conf, const response_code &code);
        void serveFile(Path &path_dir, const response_code &res_code);
        void serveDirectory(Path &path_dir);
        void serveError(const response_code &err_code);

        void generateResponse();
        void setRootDirectory(const string &root);
        void operator>>(Socket &clien_sock);
        ~Response();
};