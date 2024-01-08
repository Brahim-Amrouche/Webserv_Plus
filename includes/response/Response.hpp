/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:28:49 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/08 21:07:43 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"
#include "Cgi.hpp"

typedef enum RESPONSE_ERR_CODES
{
    E_FAILED_WRITE,
} response_err;

typedef enum RESPONSE_CODE
{
    NONE = 0,
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


class Response
{
    private:
        char (&res_buf)[HEADERS_MAX_SIZE + 1];
        Request &req;
        Cgi cgi;
        response_code code;
        ssize_t buffer_size;
        string  response_path;
    public:
        class ResponseException : TException<response_err, Response>
        {
            public:
                ResponseException(const response_err &err, Response *cln);
        };
        Response(char (&buffer)[HEADERS_MAX_SIZE + 1], Request &r);
        void listDirectory();
        void findResponseFile(const Path &index_page);
        void generateResponse();
        bool operator>>(Socket &clien_sock);
        ~Response();
};