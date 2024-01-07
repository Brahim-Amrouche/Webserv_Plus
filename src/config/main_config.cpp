/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 03:16:57 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/07 12:21:09 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void configureMimetypes()
{
    mimetypes.insert(std::pair<string, string>("application/octet-stream", "default"));
    mimetypes.insert(std::pair<string, string>("audio/aac", ".aac"));
    mimetypes.insert(std::pair<string, string>("application/x-abiword", ".abw"));
    mimetypes.insert(std::pair<string, string>("application/x-freearc", ".arc"));
    mimetypes.insert(std::pair<string, string>("image/avif", ".avif"));
    mimetypes.insert(std::pair<string, string>("video/x-msvideo", ".avi"));
    mimetypes.insert(std::pair<string, string>("application/vnd.amazon.ebook", ".azw"));
    mimetypes.insert(std::pair<string, string>("application/octet-stream", ".bin"));
    mimetypes.insert(std::pair<string, string>("image/bmp", ".bmp"));
    mimetypes.insert(std::pair<string, string>("application/x-bzip", ".bz"));
    mimetypes.insert(std::pair<string, string>("application/x-bzip2", ".bz2"));
    mimetypes.insert(std::pair<string, string>("application/x-cdf", ".cda"));
    mimetypes.insert(std::pair<string, string>("application/x-csh", ".csh"));
    mimetypes.insert(std::pair<string, string>("text/css", ".css"));
    mimetypes.insert(std::pair<string, string>("text/csv", ".csv"));
    mimetypes.insert(std::pair<string, string>("application/msword", ".doc"));
    mimetypes.insert(std::pair<string, string>("application/vnd.openxmlformats-officedocument.wordprocessingml.document", ".docx"));
    mimetypes.insert(std::pair<string, string>("application/vnd.ms-fontobject", ".eot"));
    mimetypes.insert(std::pair<string, string>("application/epub+zip", ".epub"));
    mimetypes.insert(std::pair<string, string>("application/gzip", ".gz"));
    mimetypes.insert(std::pair<string, string>("image/gif", ".gif"));
    mimetypes.insert(std::pair<string, string>("text/html", ".htm"));
    mimetypes.insert(std::pair<string, string>("text/html", ".html"));
    mimetypes.insert(std::pair<string, string>("image/vnd.microsoft.icon", ".ico"));
    mimetypes.insert(std::pair<string, string>("text/calendar", ".ics"));
    mimetypes.insert(std::pair<string, string>("application/java-archive", ".jar"));
    mimetypes.insert(std::pair<string, string>("image/jpeg", ".jpg"));
    mimetypes.insert(std::pair<string, string>("image/jpeg", ".jpeg"));
    mimetypes.insert(std::pair<string, string>("text/javascript", ".js"));
    mimetypes.insert(std::pair<string, string>("application/json", ".json"));
    mimetypes.insert(std::pair<string, string>("application/ld+json", ".jsonld"));
    mimetypes.insert(std::pair<string, string>("audio/midi, audio/x-midi", ".mid"));
    mimetypes.insert(std::pair<string, string>("audio/midi, audio/x-midi", ".midi"));
    mimetypes.insert(std::pair<string, string>("text/javascript", ".mjs"));
    mimetypes.insert(std::pair<string, string>("audio/mpeg", ".mp3"));
    mimetypes.insert(std::pair<string, string>("video/mp4", ".mp4"));
    mimetypes.insert(std::pair<string, string>("video/mpeg", ".mpeg"));
    mimetypes.insert(std::pair<string, string>("application/vnd.apple.installer+xml", ".mpkg"));
    mimetypes.insert(std::pair<string, string>("application/vnd.oasis.opendocument.presentation", ".odp"));
    mimetypes.insert(std::pair<string, string>("application/vnd.oasis.opendocument.spreadsheet", ".ods"));
    mimetypes.insert(std::pair<string, string>("application/vnd.oasis.opendocument.text", ".odt"));
    mimetypes.insert(std::pair<string, string>("audio/ogg", ".oga"));
    mimetypes.insert(std::pair<string, string>("video/ogg", ".ogv"));
    mimetypes.insert(std::pair<string, string>("application/ogg", ".ogx"));
    mimetypes.insert(std::pair<string, string>("audio/opus", ".opus"));
    mimetypes.insert(std::pair<string, string>("font/otf", ".otf"));
    mimetypes.insert(std::pair<string, string>("image/png", ".png"));
    mimetypes.insert(std::pair<string, string>("application/pdf", ".pdf"));
    mimetypes.insert(std::pair<string, string>("application/x-httpd-php", ".php"));
    mimetypes.insert(std::pair<string, string>("application/vnd.ms-powerpoint", ".ppt"));
    mimetypes.insert(std::pair<string, string>("application/vnd.openxmlformats-officedocument.presentationml.presentation", ".pptx"));
    mimetypes.insert(std::pair<string, string>("application/vnd.rar", ".rar"));
    mimetypes.insert(std::pair<string, string>("application/rtf", ".rtf"));
    mimetypes.insert(std::pair<string, string>("application/x-sh", ".sh"));
    mimetypes.insert(std::pair<string, string>("image/svg+xml", ".svg"));
    mimetypes.insert(std::pair<string, string>("application/x-tar", ".tar"));
    mimetypes.insert(std::pair<string, string>("image/tiff", ".tif"));
    mimetypes.insert(std::pair<string, string>("image/tiff", ".tiff"));
    mimetypes.insert(std::pair<string, string>("video/mp2t", ".ts"));
    mimetypes.insert(std::pair<string, string>("font/ttf", ".ttf"));
    mimetypes.insert(std::pair<string, string>("text/plain", ".txt"));
    mimetypes.insert(std::pair<string, string>("application/vnd.visio", ".vsd"));
    mimetypes.insert(std::pair<string, string>("audio/wav", ".wav"));
    mimetypes.insert(std::pair<string, string>("audio/webm", ".weba"));
    mimetypes.insert(std::pair<string, string>("video/webm", ".webm"));
    mimetypes.insert(std::pair<string, string>("image/webp", ".webp"));
    mimetypes.insert(std::pair<string, string>("font/woff", ".woff"));
    mimetypes.insert(std::pair<string, string>("font/woff2", ".woff2"));
    mimetypes.insert(std::pair<string, string>("application/xhtml+xml", ".xhtml"));
    mimetypes.insert(std::pair<string, string>("application/vnd.ms-excel", ".xls"));
    mimetypes.insert(std::pair<string, string>("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", ".xlsx"));
    mimetypes.insert(std::pair<string, string>("application/xml", ".xml"));
    mimetypes.insert(std::pair<string, string>("application/vnd.mozilla.xul+xml", ".xul"));
    mimetypes.insert(std::pair<string, string>("application/zip", ".zip"));
    mimetypes.insert(std::pair<string, string>("video/3gpp;", ".3gp"));
    mimetypes.insert(std::pair<string, string>("video/3gpp2;", ".3g2"));
    mimetypes.insert(std::pair<string, string>("application/x-7z-compressed", ".7z"));
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
        parser->normalizeServerConfigs();
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
    return server_sockets;
}