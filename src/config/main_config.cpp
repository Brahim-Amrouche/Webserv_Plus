/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 03:16:57 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/11 19:00:03 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void configureMimetypes()
{
    mimetypes.insert(std::pair<string, string>("default", "application/octet-stream"));
    mimetypes.insert(std::pair<string, string>(".aac", "audio/aac"));
    mimetypes.insert(std::pair<string, string>(".abw", "application/x-abiword"));
    mimetypes.insert(std::pair<string, string>(".arc", "application/x-freearc"));
    mimetypes.insert(std::pair<string, string>(".avif", "image/avif"));
    mimetypes.insert(std::pair<string, string>(".avi", "video/x-msvideo"));
    mimetypes.insert(std::pair<string, string>(".azw", "application/vnd.amazon.ebook"));
    mimetypes.insert(std::pair<string, string>(".bin", "application/octet-stream"));
    mimetypes.insert(std::pair<string, string>(".bmp", "image/bmp"));
    mimetypes.insert(std::pair<string, string>(".bz", "application/x-bzip"));
    mimetypes.insert(std::pair<string, string>(".bz2", "application/x-bzip2"));
    mimetypes.insert(std::pair<string, string>(".cda", "application/x-cdf"));
    mimetypes.insert(std::pair<string, string>(".csh", "application/x-csh"));
    mimetypes.insert(std::pair<string, string>(".css", "text/css"));
    mimetypes.insert(std::pair<string, string>(".csv", "text/csv"));
    mimetypes.insert(std::pair<string, string>(".doc", "application/msword"));
    mimetypes.insert(std::pair<string, string>(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
    mimetypes.insert(std::pair<string, string>(".eot", "application/vnd.ms-fontobject"));
    mimetypes.insert(std::pair<string, string>(".epub", "application/epub+zip"));
    mimetypes.insert(std::pair<string, string>(".gz", "application/gzip"));
    mimetypes.insert(std::pair<string, string>(".gif", "image/gif"));
    mimetypes.insert(std::pair<string, string>(".htm", "text/html"));
    mimetypes.insert(std::pair<string, string>(".html", "text/html"));
    mimetypes.insert(std::pair<string, string>(".ico", "image/vnd.microsoft.icon"));
    mimetypes.insert(std::pair<string, string>(".ics", "text/calendar"));
    mimetypes.insert(std::pair<string, string>(".jar", "application/java-archive"));
    mimetypes.insert(std::pair<string, string>(".jpg", "image/jpeg"));
    mimetypes.insert(std::pair<string, string>(".jpeg", "image/jpeg"));
    mimetypes.insert(std::pair<string, string>(".js", "text/javascript"));
    mimetypes.insert(std::pair<string, string>(".json", "application/json"));
    mimetypes.insert(std::pair<string, string>(".jsonld", "application/ld+json"));
    mimetypes.insert(std::pair<string, string>(".mid", "audio/midi, audio/x-midi"));
    mimetypes.insert(std::pair<string, string>(".midi", "audio/midi, audio/x-midi"));
    mimetypes.insert(std::pair<string, string>(".mjs", "text/javascript"));
    mimetypes.insert(std::pair<string, string>(".mp3", "audio/mpeg"));
    mimetypes.insert(std::pair<string, string>(".mp4", "video/mp4"));
    mimetypes.insert(std::pair<string, string>(".mpeg", "video/mpeg"));
    mimetypes.insert(std::pair<string, string>(".mpkg", "application/vnd.apple.installer+xml"));
    mimetypes.insert(std::pair<string, string>(".odp", "application/vnd.oasis.opendocument.presentation"));
    mimetypes.insert(std::pair<string, string>(".ods", "application/vnd.oasis.opendocument.spreadsheet"));
    mimetypes.insert(std::pair<string, string>(".odt", "application/vnd.oasis.opendocument.text"));
    mimetypes.insert(std::pair<string, string>(".oga", "audio/ogg"));
    mimetypes.insert(std::pair<string, string>(".ogv", "video/ogg"));
    mimetypes.insert(std::pair<string, string>(".ogx", "application/ogg"));
    mimetypes.insert(std::pair<string, string>(".opus", "audio/opus"));
    mimetypes.insert(std::pair<string, string>(".otf", "font/otf"));
    mimetypes.insert(std::pair<string, string>(".png", "image/png"));
    mimetypes.insert(std::pair<string, string>(".pdf", "application/pdf"));
    mimetypes.insert(std::pair<string, string>(".php", "application/x-httpd-php"));
    mimetypes.insert(std::pair<string, string>(".ppt", "application/vnd.ms-powerpoint"));
    mimetypes.insert(std::pair<string, string>(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
    mimetypes.insert(std::pair<string, string>(".rar", "application/vnd.rar"));
    mimetypes.insert(std::pair<string, string>(".rtf", "application/rtf"));
    mimetypes.insert(std::pair<string, string>(".sh", "application/x-sh"));
    mimetypes.insert(std::pair<string, string>(".svg", "image/svg+xml"));
    mimetypes.insert(std::pair<string, string>(".tar", "application/x-tar"));
    mimetypes.insert(std::pair<string, string>(".tif", "image/tiff"));
    mimetypes.insert(std::pair<string, string>(".tiff", "image/tiff"));
    mimetypes.insert(std::pair<string, string>(".ts", "video/mp2t"));
    mimetypes.insert(std::pair<string, string>(".ttf", "font/ttf"));
    mimetypes.insert(std::pair<string, string>(".txt", "text/plain"));
    mimetypes.insert(std::pair<string, string>(".vsd", "application/vnd.visio"));
    mimetypes.insert(std::pair<string, string>(".wav", "audio/wav"));
    mimetypes.insert(std::pair<string, string>(".weba", "audio/webm"));
    mimetypes.insert(std::pair<string, string>(".webm", "video/webm"));
    mimetypes.insert(std::pair<string, string>(".webp", "image/webp"));
    mimetypes.insert(std::pair<string, string>(".woff", "font/woff"));
    mimetypes.insert(std::pair<string, string>(".woff2", "font/woff2"));
    mimetypes.insert(std::pair<string, string>(".xhtml", "application/xhtml+xml"));
    mimetypes.insert(std::pair<string, string>(".xls", "application/vnd.ms-excel"));
    mimetypes.insert(std::pair<string, string>(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
    mimetypes.insert(std::pair<string, string>(".xml", "application/xml"));
    mimetypes.insert(std::pair<string, string>(".xul", "application/vnd.mozilla.xul+xml"));
    mimetypes.insert(std::pair<string, string>(".zip", "application/zip"));
    mimetypes.insert(std::pair<string, string>(".3gp", "video/3gpp;"));
    mimetypes.insert(std::pair<string, string>(".3g2", "video/3gpp2;"));
    mimetypes.insert(std::pair<string, string>(".7z", "application/x-7z-compressed"));
}

void configureResponseCodes()
{
    response_handle.insert(std::pair<response_code, string>(RES_CONTINUE, "Continue"));
    response_handle.insert(std::pair<response_code, string>(RES_SWITCHING_PROTOCOLS, "Switching Protocols"));
    response_handle.insert(std::pair<response_code, string>(RES_PROCESSING, "Processing"));
    response_handle.insert(std::pair<response_code, string>(RES_EARLY_HINTS, "Early Hints"));
    response_handle.insert(std::pair<response_code, string>(RES_OK, "OK"));
    response_handle.insert(std::pair<response_code, string>(RES_CREATED, "Created"));
    response_handle.insert(std::pair<response_code, string>(RES_ACCEPTED, "Accepted"));
    response_handle.insert(std::pair<response_code, string>(RES_NON_AUTHORITATIVE_INFORMATION, "Non-Authoritative Information"));
    response_handle.insert(std::pair<response_code, string>(RES_NO_CONTENT, "No Content"));
    response_handle.insert(std::pair<response_code, string>(RES_RESET_CONTENT, "Reset Content"));
    response_handle.insert(std::pair<response_code, string>(RES_PARTIAL_CONTENT, "Partial Content"));
    response_handle.insert(std::pair<response_code, string>(RES_MULTI_STATUS, "Multi-Status"));
    response_handle.insert(std::pair<response_code, string>(RES_ALREADY_REPORTED, "Already Reported"));
    response_handle.insert(std::pair<response_code, string>(RES_IM_USED, "IM Used"));
    response_handle.insert(std::pair<response_code, string>(RES_MULTIPLE_CHOICES, "Multiple Choices"));
    response_handle.insert(std::pair<response_code, string>(RES_MOVE_PERMANENTLY, "Moved Permanently"));
    response_handle.insert(std::pair<response_code, string>(RES_FOUND, "Found"));
    response_handle.insert(std::pair<response_code, string>(RES_SEE_OTHER, "See Other"));
    response_handle.insert(std::pair<response_code, string>(RES_NOT_MODIFIED, "Not Modified"));
    response_handle.insert(std::pair<response_code, string>(RES_USE_PROXY, "Use Proxy"));
    response_handle.insert(std::pair<response_code, string>(RES_SWITCH_PROXY, "Switch Proxy"));
    response_handle.insert(std::pair<response_code, string>(RES_TEMPORARY_REDIRECT, "Temporary Redirect"));
    response_handle.insert(std::pair<response_code, string>(RES_PERMANENT_REDIRECT, "Permanent Redirect"));
    response_handle.insert(std::pair<response_code, string>(RES_BAD_REQUEST, "Bad Request"));
    response_handle.insert(std::pair<response_code, string>(RES_UNAUTHORIZED, "Unauthorized"));
    response_handle.insert(std::pair<response_code, string>(RES_PAYMENT_REQUIRED, "Payment Required"));
    response_handle.insert(std::pair<response_code, string>(RES_FORBIDDEN, "Forbidden"));
    response_handle.insert(std::pair<response_code, string>(RES_NOT_FOUND, "Not Found"));
    response_handle.insert(std::pair<response_code, string>(RES_METHOD_NOT_ALLOWED, "Method Not Allowed"));
    response_handle.insert(std::pair<response_code, string>(RES_NOT_ACCEPTABLE, "Not Acceptable"));
    response_handle.insert(std::pair<response_code, string>(RES_PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"));
    response_handle.insert(std::pair<response_code, string>(RES_REQUEST_TIMEOUT, "Request Timeout"));
    response_handle.insert(std::pair<response_code, string>(RES_CONFLICT, "Conflict"));
    response_handle.insert(std::pair<response_code, string>(RES_GONE, "Gone"));
    response_handle.insert(std::pair<response_code, string>(RES_LENGTH_REQUIRED, "Length Required"));
    response_handle.insert(std::pair<response_code, string>(RES_PRECONDITION_FAILED, "Precondition Failed"));
    response_handle.insert(std::pair<response_code, string>(RES_PAYLOAD_TOO_LARGE, "Payload Too Large"));
    response_handle.insert(std::pair<response_code, string>(RES_URI_TOO_LONG, "URI Too Long"));
    response_handle.insert(std::pair<response_code, string>(RES_UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type"));
    response_handle.insert(std::pair<response_code, string>(RES_RANGE_NOT_SATISFIABLE, "Range Not Satisfiable"));
    response_handle.insert(std::pair<response_code, string>(RES_EXPECTATION_FAILED, "Expectation Failed"));
    response_handle.insert(std::pair<response_code, string>(RES_IM_A_TEAPOT, "I'm a teapot"));
    response_handle.insert(std::pair<response_code, string>(RES_MISDIRECTED_REQUEST, "Misdirected Request"));
    response_handle.insert(std::pair<response_code, string>(RES_UNPROCESSABLE_ENTITY, "Unprocessable Entity"));
    response_handle.insert(std::pair<response_code, string>(RES_LOCKED, "Locked"));
    response_handle.insert(std::pair<response_code, string>(RES_FAILED_DEPENDENCY, "Failed Dependency"));
    response_handle.insert(std::pair<response_code, string>(RES_UPGRADE_REQUIRED, "Upgrade Required"));
    response_handle.insert(std::pair<response_code, string>(RES_PRECONDITION_REQUIRED, "Precondition Required"));
    response_handle.insert(std::pair<response_code, string>(RES_TOO_MANY_REQUESTS, "Too Many Requests"));
    response_handle.insert(std::pair<response_code, string>(RES_REQUEST_HEADER_FIELDS_TOO_LARGE, "Request Header Fields Too Large"));
    response_handle.insert(std::pair<response_code, string>(RES_UNAVAILABLE_FOR_LEGAL_REASONS, "Unavailable For Legal Reasons"));
    response_handle.insert(std::pair<response_code, string>(RES_INTERNAL_SERVER_ERROR, "Internal Server Error"));
    response_handle.insert(std::pair<response_code, string>(RES_NOT_IMPLEMENTED, "Not Implemented"));
    response_handle.insert(std::pair<response_code, string>(RES_BAD_GATEWAY, "Bad Gateway"));
    response_handle.insert(std::pair<response_code, string>(RES_SERVICE_UNAVAILABLE, "Service Unavailable"));
    response_handle.insert(std::pair<response_code, string>(RES_GATEWAY_TIMEOUT, "Gateway Timeout"));
    response_handle.insert(std::pair<response_code, string>(RES_HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"));
    response_handle.insert(std::pair<response_code, string>(RES_VARIANT_ALSO_NEGOTIATES, "Variant Also Negotiates"));
    response_handle.insert(std::pair<response_code, string>(RES_INSUFFICIENT_STORAGE, "Insufficient Storage"));
    response_handle.insert(std::pair<response_code, string>(RES_LOOP_DETECTED, "Loop Detected"));
    response_handle.insert(std::pair<response_code, string>(RES_NOT_EXTENDED, "Not Extended"));
    response_handle.insert(std::pair<response_code, string>(RES_NETWORK_AUTHENTICATION_REQUIRED, "Network Authentication Required"));
}

deque<ServerSocket>   *configure(const char *path)
{
    TokenizeInput *tokenizer = NULL;
    list<string> *tokens = NULL;
    try
    {
        tokenizer = new TokenizeInput(path);
        tokens = tokenizer->getTokensList();
        delete tokenizer;
        tokenizer = NULL;
    }
    catch(const TokenizeInput::TokenizeInputExceptions& e)
    {  
        cout << e.what() << endl;
        return NULL;
    }
    ConfigParser *parser = new ConfigParser(tokens);
    deque<ServerSocket> *server_sockets = NULL;
    try 
    {
        ConfigParser::TokenIt it = parser->getTokenStart();
        parser->parseConfig(it);
        parser->generateServerSockets();
        server_sockets = parser->getServerSockets();
        delete parser;
        parser = NULL;
    }
    catch (const ConfigParser::ConfigParserException &e)
    {
        cout << e.what() << endl;
        return NULL;
    }
    catch (const Socket::SocketExceptions &e)
    {
        cout << e.what() << endl;
        delete parser;
        return NULL;
    }
    cleanup_data.cleanup_server_sock = server_sockets;
    configureMimetypes();
    configureResponseCodes();
    return server_sockets;
}