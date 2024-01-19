
#pragma once
#include <fstream>
#include "Request.hpp"

using std::ofstream;

typedef enum E_BODY_ERR_CODES
{
    E_INVALID_BODY_CONFIG,
    E_INVALID_BODY_HEADERS,
    E_BODY_SIZE_OVERFLOW,
    E_UNABLE_TO_OPEN_TMP_FILE,
    E_BODY_READING,
} body_err;

enum BODY_MODE
{
    M_NO_CONF,
    M_NO_BODY,
    M_CHUNKED,
    M_IDENTITY,
    M_CONTENT_LENGTH,
};

class Body
{
    private:
        string  &req_id;
        char   (&buffer)[HEADERS_MAX_SIZE + 1];
        ServerConfiguration *&conf;
        Headers &headers;
        ssize_t max_config_size;
        ssize_t content_length;
        ssize_t body_size;
        BODY_MODE mode;
        ofstream body_file;
        bool    body_done;
    public:
        class BodyException: public TException< body_err, Body>
        {
            public:
                BodyException(const body_err &err, Body *cln); 
        };
        Body(string &req_id, char (&buffer)[HEADERS_MAX_SIZE + 1], ServerConfiguration *(&c), Headers &h);
        void fromConfig();
        void fromHeaders();
        void configBody();
        bool readChunked(ssize_t &buffer_size);
        bool readIdentity(ssize_t &buffer_size);
        bool readContentLength(ssize_t &buffer_size);
        bool operator<<(ssize_t &buffer_size);
        BODY_MODE getMode()
        {
            return (mode);
        };
        bool getBodyDone()
        {
            return (body_done);
        };
        bool getBodySize()
        {
            return (body_size);
        };
        bool bodyDone();
        ~Body();
};