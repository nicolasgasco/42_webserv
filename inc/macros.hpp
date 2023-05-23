#pragma once

#define BACKLOG_DEFAULT 100
#define CGI_BIN_PATH "cgi-bin"
#define CGI_BUF_LEN 20000
#define CGI_MAX_ENV_VARS 20
#define CRLF_LEN 2
#define DEFAULT_INDEX_PAGE "index.html"
#define DIR_PATH "dir"
#define ERRORS_PATH "error"
#define GALLERY_STORAGE_PATH "public/pictures"
#define HTTP_PROTOCOL "HTTP/1.1"
#define LONGEST_METHOD "DELETE"
#define LONGEST_URI 124
#define MIN_FD 3
#define FD_READ 0
#define FD_WRITE 1
#define PUBLIC_PATH "public"
#define PYTHON3_PATH "/usr/bin/python3"
#define SHELL_PATH "/bin/bash"
#define REC_BUFF_SIZE 300000
#define SERVER_TIMEOUT 30
#define WHITESPACES " \t\v\f\r\n"

// HTTP CODES
#define HTTP_200_CODE 200
#define HTTP_200_REASON "OK"
#define HTTP_301_CODE 301
#define HTTP_301_REASON "Moved Permanently"
#define HTTP_400_CODE 400
#define HTTP_400_REASON "Bad Request"
#define HTTP_403_CODE 403
#define HTTP_403_REASON "Forbidden"
#define HTTP_404_CODE 404
#define HTTP_404_REASON "Not Found"
#define HTTP_405_CODE 405
#define HTTP_405_REASON "Method Not Allowed"
#define HTTP_413_CODE 413
#define HTTP_413_REASON "Content Too Large"
#define HTTP_414_CODE 414
#define HTTP_414_REASON "URI Too Long"
#define HTTP_500_CODE 500
#define HTTP_500_REASON "Internal Server Error"
#define HTTP_501_CODE 501
#define HTTP_501_REASON "Not Implemented"

// HTTP HEADERS
#define CONTENT_LENGTH "Content-Length"
#define CONTENT_TYPE "Content-Type"
#define HOST "Host"
#define TRANSFER_ENCODING "Transfer-Encoding"

// Output colors
#define GREY "\033[30;1m"
#define NC "\033[0m"
#define YELLOW "\033[0;33m"
