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
std::string const HttpService::build_headers(int const &content_len, class Webserver *webserver) const
{
    std::string headers;

    std::string date = "Date: " + get_gmt_time() + "\r\n";

    std::string server_name;

    for (std::vector<Server>::iterator it = webserver->_server.begin(); it != webserver->_server.end(); it++)
    {
        Server srv_data = *it;
        server_name = srv_data.get_server_name();
        std::cout << "Server: " << std::string(server_name) << std::endl;
    }

    std::string server = "Server: " + std::string(server_name) + "/1.0" + "\r\n";

    std::string content_length = "Content-Length: " + std::to_string(content_len) + "\r\n";

    // TODO check if Content-Type is creating issues
    // std::string content_type = this->_get_content_type(this->_req.get_req_line().target);

    headers += date;
    headers += server;
    headers += content_length;
    // headers += content_type;

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
