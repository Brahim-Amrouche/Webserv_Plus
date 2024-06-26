
#pragma once
#include "./server/server.hpp"
#include "./helpers/helpers.hpp"
#include "./config/config.hpp"
#include "./request/Request.hpp"
#include "./response/Response.hpp"
#include <climits>

#define DEFAULT_CONFIG "/config/default_config.conf"
#ifndef DEFAULT_ROOT
    #define DEFAULT_ROOT "/"
#endif
#define DEFAULT_CONTENT "/content"
#define DEFAULT_ERROR_PAGES "/content/error_pages"
#define DEFAULT_TMP_FOLDER "/tmp"
#define DEFAULT_INDEX "index.html"

#define DEFAULT_ERROR_HTML "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Error: "
#define DEFAULT_LIST_HTML "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Listing of "

#define PHP_CGI_PATH "/usr/bin/php-cgi"
#define PYTHON_CGI_PATH "/usr/bin/python3"

#define LOADBALANCER_CHECK_DELAY 1000 // in milliseconds
#define MAX_TIMEOUT 10 // in seconds

extern map<string, string> mimetypes;
extern map<response_code, string> response_handle;

typedef struct s_cleanup
{
    deque<ServerSocket>   *cleanup_server_sock;
    LoadBalancer *cleanup_loadbalancer;
}   t_cleanup;

extern t_cleanup cleanup_data;
