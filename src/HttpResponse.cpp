#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest &req, RouterService const &router) : _router(router), _req(req)
{
    if (this->_req.has_error())
        this->_build_error_res();
    else
    {
        std::string method = this->_req.get_req_line().method;
        if (method == "GET")
            this->_build_ok_res();
        else if (method == "POST")
        {
            // To be built
            std::cout << "POST request" << std::endl;
            this->_build_post_request();
        }
        else if (method == "DELETE")
        {
            // To be built
        }
    }

    // TODO delete this when build is done
    std::cout << *this << std::endl
              << std::endl;
}

void HttpResponse::_build_post_request()
{
    std::string buff = this->_req.get_buff();

    std::cout << "Len is " << buff.length() << std::endl;

    std::string content_type = this->_req.get_attrs().at("Content-Type");
    std::string boundary = content_type.substr(content_type.find("=") + 1);
    std::cout << "boundary is " << boundary << std::endl;

    size_t firstBoundaryPos = buff.find(boundary);
    std::cout << "First boundary is " << firstBoundaryPos << std::endl;
    std::string right = buff.substr(firstBoundaryPos + boundary.length());

    std::string filename = right.substr(right.find("filename=\"") + 10);
    filename = filename.substr(0, filename.find("\""));
    std::cout << "Filename " << filename << "." << std::endl;

    size_t secondBoundaryPos = right.find(boundary);
    std::cout << "Second boundary is " << secondBoundaryPos << std::endl;

    std::string three = right.substr(secondBoundaryPos + boundary.length());
    three = three.substr(three.find("\r\n\r\n") + 4);
    three = three.substr(0, three.find(boundary) - 2);
    std::cout << "Cazzo >" << three << "<" << std::endl;

    std::ofstream target_file(filename);
    target_file << three;
    target_file.close();
}

HttpResponse::~HttpResponse()
{
}

void HttpResponse::_build_error_res()
{
    int reqErrCode = this->_req.gett_err().code;
    std::string reqErrMessage = this->_req.gett_err().message;

    this->set_status_line(reqErrCode, reqErrMessage);
    this->_buff = this->_build_status_line();

    // TODO check if it's possible to always send HTML
    if (this->_req.is_html_req())
    {
        std::ifstream file(this->_router.get_def_err_file_path());

        std::string res_file = this->_build_file(file);
        this->_replace_var_in_page(res_file, "{{code}}", std::to_string(reqErrCode));
        this->_replace_var_in_page(res_file, "{{message}}", reqErrMessage);

        this->_buff += res_file;
    }
}

void HttpResponse::_build_ok_res()
{
    std::string file_path = this->_router.get_file_path(this->_req);

    std::ifstream file(file_path);

    if (file)
    {
        this->set_status_line(200, "OK");
        this->_buff = this->_build_file(file);
    }
    else
    {
        this->set_status_line(404, "Not Found");
        std::ifstream file_404(this->_router.get_404_file_path());
        if (file_404)
            this->_buff = this->_build_file(file_404);
        else
            std::cerr << RED << "Error: missing 404.html file" << NC << std::endl;
    }

    // Pre-append status line
    this->_buff.insert(0, this->_build_status_line());
}

std::string HttpResponse::_build_file(std::ifstream const &file)
{
    std::string message_body;

    std::ostringstream data_stream;

    data_stream << file.rdbuf();

    message_body += "\r\n";
    message_body += data_stream.str();

    return message_body;
}

std::string HttpResponse::_build_status_line() const
{
    std::string status_line;
    status_line += this->_status_line.version + " " + std::to_string(this->_status_line.code) + " " + this->_status_line.reason;
    status_line += "\r\n";
    return status_line;
}

void HttpResponse::_replace_var_in_page(std::string &file, std::string const var, std::string const value) const
{
    while (true)
    {
        size_t posCode = file.find(var);
        if (posCode == std::string::npos)
            return;

        file.erase(posCode, var.length());
        file.insert(posCode, value);
    }
}

StatusLine const &HttpResponse::get_status_line() const
{
    return this->_status_line;
}

std::string const &HttpResponse::get_buff() const
{
    return this->_buff;
}

void HttpResponse::set_status_line(int const &code, std::string const &reason)
{
    this->_status_line.version = std::string(HTTP_PROTOCOL);
    this->_status_line.code = int(code);
    this->_status_line.reason = std::string(reason);
}

std::ostream &operator<<(std::ostream &os, HttpResponse &std)
{
    std::cout << std::endl;

    std::cout << "STATUS LINE:" << std::endl;
    std::cout << "version: " << YELLOW << "." << NC << std.get_status_line().version << YELLOW << "." << NC << std::endl;
    std::cout << "status-code: " << YELLOW << "." << NC << std.get_status_line().code << YELLOW << "." << NC << std::endl;
    std::cout << "reason-phrase: " << YELLOW << "." << NC << std.get_status_line().reason << YELLOW << "." << NC << std::endl;

    std::cout << std::endl;

    return os;
}