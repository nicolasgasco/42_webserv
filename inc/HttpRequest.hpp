#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "HttpService.hpp"
#include "Location.hpp"
#include "Webserver.hpp"

#include "macros.hpp"
#include "utils.hpp"

struct ReqLine
{
	std::string method;
	std::string target;
	std::string raw_target;
	std::string version;
};

struct ReqErr
{
	int code;
	std::string message;
};

class ServerConnection;

class HttpRequest
{
	friend class ServerConnection;

public:
	HttpRequest();
	~HttpRequest();

	// Getters
	std::map<std::string, std::string> const &get_attrs() const;
	std::vector<char> const &get_body() const;
	ReqErr const &gett_err() const;
	std::map<std::string, std::string> const &get_params() const;
	ReqLine const &get_req_line() const;
	bool get_is_redirection() const;

	// Setters
	void add_to_body(std::vector<char> &buff);
	void set_body(std::vector<char> buff);

	// Methods
	void parse_req(Server const *server, Webserver *webserver);
	void output_status();
	void reset();
	void set_err(int const &code, std::string const &message);

	// Computed properties
	bool has_body() const;
	bool has_error() const;
	bool is_cgi_req() const;
	bool is_dir_req() const;
	bool is_html_req() const;

	std::string port_browser;

private:
	std::map<std::string, std::string> _attrs, _params;
	std::vector<char> _body;
	ReqLine _req_line;
	ReqErr _err;
	bool _is_redirect;

	void _parse_attr_line(std::string const &line);
	void _parse_method(std::string &line);
	int _parse_req_line(std::string &line, Server const *server);
	void _parse_target(std::string &line, Server const *server);
	void _parse_version(std::string &line);
	void _validate_req_with_config(Webserver *webserver);

	// Computed properties
	bool _is_method_supported(std::string method) const;
	bool _is_target_redirection(std::string const &target, Server *server) const;
};

std::ostream &operator<<(std::ostream &os, HttpRequest &std);
