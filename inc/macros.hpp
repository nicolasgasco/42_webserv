#pragma once

#define ACCEPT_HTML "text/html,application/xhtml+xml,application/xml"
#define BACKLOG_DEFAULT 100
#define CGI_BIN_PATH "cgi-bin"
#define CGI_BUF_LEN 20000
#define CGI_MAX_ENV_VARS 20
#define CRLF_LEN 2
#define DIR_PATH "dir"
#define ERRORS_PATH "error"
#define GALLERY_STORAGE_PATH "public/gallery/pictures"
#define HTTP_PROTOCOL "HTTP/1.1"
#define LONGEST_METHOD "DELETE"
#define LONGEST_URI 124 // Replace with computed value
#define MIN_FD 3
#define PIPE_BODY 1
#define PIPE_RES 0
#define FD_READ 0
#define FD_WRITE 1
#define POST_BODY_FILENAME_DELIM "filename=\""
#define POST_BODY_BOUNDARY "boundary="
#define PUBLIC_PATH "public"
#define PYTHON3_PATH "/usr/bin/python3"
#define SHELL_PATH "/bin/bash"
#define REC_BUFF_SIZE 300000
#define SERVER_TIMEOUT 30
#define GALLERY_SUCCESS_TEMPLATE_PATH "public/gallery/upload/success.html"
#define WHITESPACES " \t\v\f\r\n"

// HTTP CODES
#define HTTP_200_CODE 200
#define HTTP_200_REASON "OK"
#define HTTP_301_CODE 301
#define HTTP_301_REASON "Moved Permanently"
#define HTTP_400_CODE 400
#define HTTP_400_REASON "Bad Request"
#define HTTP_401_CODE 401
#define HTTP_401_REASON "Not Authorized"
#define HTTP_404_CODE 404
#define HTTP_404_REASON "Not Found"
#define HTTP_405_CODE 405
#define HTTP_405_REASON "Method Not Allowed"
#define HTTP_409_CODE 409
#define HTTP_409_REASON "Conflict"
#define HTTP_413_CODE 413
#define HTTP_413_REASON "Content Too Large"
#define HTTP_414_CODE 414
#define HTTP_414_REASON "URI Too Long"
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
