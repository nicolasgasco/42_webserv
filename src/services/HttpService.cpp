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
    if (target.find("%3C") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%3C"), 3, "<");
    if (target.find("%3E") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%3E"), 3, ">");
    if (target.find("%23") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%23"), 3, "#");
    if (target.find("%25") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%25"), 3, "%");
    if (target.find("%7B") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%7B"), 3, "{");
    if (target.find("%7D") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%7D"), 3, "}");
    if (target.find("%7C") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%7C"), 3, "|");
    if (target.find("%5C") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%5C"), 3, "\\");
    if (target.find("%5E") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%5E"), 3, "^");
    if (target.find("%7E") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%7E"), 3, "~");
    if (target.find("%5B") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%5B"), 3, "[");
    if (target.find("%5D") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%5D"), 3, "]");
    if (target.find("%60") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%60"), 3, "`");
    if (target.find("%3B") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%3B"), 3, ";");
    if (target.find("%2F") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%2F"), 3, "/");
    if (target.find("%3F") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%3F"), 3, "?");
    if (target.find("%3A") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%3A"), 3, ":");
    if (target.find("%40") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%40"), 3, "@");
    if (target.find("%3D") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%3D"), 3, "=");
    if (target.find("%26") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%26"), 3, "&");
    if (target.find("%24") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%24"), 3, "$");
    if (target.find("%2B") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%2B"), 3, "+");
    if (target.find("%2C") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%2C"), 3, ",");
    if (target.find("%22") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%22"), 3, "\"");
    if (target.find("%C3%97") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%C3%97"), 6, "×");
    if (target.find("%C3%B7") != std::string::npos)
        expanded_target = expanded_target.replace(expanded_target.find("%C3%B7"), 6, "÷");

    return expanded_target;
}