#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "dev_utils.hpp" // TODO remove this after build is done
#include "macros.hpp"
#include "utils.hpp"

struct ReqLine
{
    std::string method;
    std::string target;
    std::string version;
};

struct ReqErr
{
    int code;
    std::string message;
};

class HttpRequest
{
private:
    std::map<std::string, std::string> _attrs, _params;
    std::vector<char> _body;
    std::string _post_req_file_name;
    ReqLine _req_line;
    ReqErr _err;

public:
    HttpRequest();
    ~HttpRequest();

    // Getters
    std::map<std::string, std::string> const &get_attrs() const;
    std::vector<char> const &get_body() const;
    ReqErr const &gett_err() const;
    std::map<std::string, std::string> const &get_params() const;
    std::string const &get_post_req_file_name() const;
    ReqLine const &get_req_line() const;

    // Setters
    void set_body(std::vector<char> &buff);

    // Methods
    void parse_post_req_body();
    void parse_post_req_file_name(std::vector<char> const &body);
    void parse_req();
    void output_status();
    void reset();

    // Computed properties
    bool has_body() const;
    bool has_error() const;
    bool has_query_params() const;
    bool is_cgi_req() const;
    bool is_dir_req() const;
    bool is_html_req() const;

private:
    void _parse_attr_line(std::string const &line);
    void _parse_method(std::string &line);
    std::string const _parse_post_req_boundary() const;
    void _parse_query_params(std::string &target);
    void _parse_req_line(std::string &line);
    void _parse_target(std::string &line);
    void _parse_version(std::string &line);

    void _set_err(int const &code, std::string const &message);

    // Computed properties
    bool _is_method_supported() const;
};

// TODO remove when not required anymore
std::ostream &operator<<(std::ostream &os, HttpRequest &std);
std::ostream &operator<<(std::ostream &os, ReqErr &std);
