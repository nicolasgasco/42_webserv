#include "HttpService.hpp"
#include "Webserver.hpp"
#include "Server.hpp"

HttpService::HttpService()
{
}

HttpService::~HttpService()
{
}

/**
 * Writes file into std::string.
 * @param file File stream.
 */
std::string const HttpService::build_file(std::ifstream const &file) const
{
    std::string message_body;

    std::ostringstream data_stream;

    data_stream << file.rdbuf();

    message_body += "\r\n";
    message_body += data_stream.str();

    return message_body;
}

/**
 * Build headers for a response.
 * @param content_len Len of content to be sent.
 */
std::string const HttpService::build_headers(int const &content_len, std::string const &server_name, std::string const &content_type) const
{
    std::string headers;

    std::string date = "Date: " + get_gmt_time() + "\r\n";

    std::string server = "Server: " + std::string(server_name) + "/1.0" + "\r\n";

    std::string content_length = "Content-Length: " + std::to_string(content_len) + "\r\n";

    headers += date;
    headers += server;
    headers += content_length;
    if (content_type.size())
        headers += content_type;

    return headers;
}

/**
 * Writes a formatted status line.
 * @param version Response HTTP version.
 * @param code Response code.
 * @param reason Response message.
 */
std::string const HttpService::build_status_line(std::string const &version, int const &code, std::string const &reason) const
{
    std::string status_line;
    status_line += version + " " + std::to_string(code) + " " + reason;
    status_line += "\r\n";
    return status_line;
}

/**
 * Expands special characters in a string.
 * @param target String to be expanded.
 */
std::string const HttpService::decode_whitespace(std::string const &target) const
{
    std::string expanded_target = target;

    if (target.find("%20") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%20"), 3, " ");

    return expanded_target;
}