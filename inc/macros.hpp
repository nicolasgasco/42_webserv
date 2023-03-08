#pragma once

#define ACCEPT_HTML "text/html,application/xhtml+xml,application/xml"
#define BACKLOG_DEFAULT 10
#define CGI_BUF_LEN 20000
#define CHROME_BODY_BOUNDARY "------WebKitFormBoundary"
#define CRLF_LEN 2
#define DIR_PATH "dir"
#define ERRORS_PATH "error"
#define GALLERY_STORAGE_PATH "public/gallery/pictures"
#define HTTP_PROTOCOL "HTTP/1.1"
#define LONGEST_METHOD "DELETE" // Replace with computed value
#define LONGEST_URI 124         // Replace with computed value
#define MIN_FD 4
#define PIPE_READ 0
#define PIPE_WRITE 1
#define PUBLIC_PATH "public"
#define PYTHON3_PATH "/usr/bin/python3"
#define REC_BUFF_SIZE 300000 // TODO change to parametrized values
#define SERVER_TIMEOUT 30
#define GALLERY_SUCCESS_TEMPLATE_PATH "public/gallery/upload/success.html"
#define WHITESPACES " \t\v\f\r\n"

// HTTP CODES
#define HTTP_200_CODE 200
#define HTTP_200_REASON "OK"
#define HTTP_401_CODE 401
#define HTTP_401_REASON "Not Authorized"
#define HTTP_404_CODE 404
#define HTTP_404_REASON "Not Found"
#define HTTP_501_CODE 501
#define HTTP_501_REASON "Not Implemented"

// HTTP HEADERS
#define CONTENT_LENGTH "Content-Length"
#define TRANSFER_ENCODING "Transfer-Encoding"
// Output colors
#define GREY "\033[30;1m"
#define NC "\033[0m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
