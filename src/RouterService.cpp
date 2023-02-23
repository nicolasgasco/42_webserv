#include "RouterService.hpp"

RouterService::RouterService()
{
}

RouterService::~RouterService()
{
}

std::string const RouterService::get_file_path(HttpRequest const &req) const
{
    std::string target = req.get_req_line().target;

    bool does_contain_dot = target.find(".") != std::string::npos;
    if (req.is_html_req() && !does_contain_dot)
        return build_path(PUBLIC_PATH, target, "index.html");
    else
        return build_path(PUBLIC_PATH, target);
}

std::string const RouterService::get_404_file_path() const
{
    return build_path(PUBLIC_PATH, ERRORS_PATH, "404.html");
}

std::string const RouterService::get_def_err_file_path() const
{
    return build_path(PUBLIC_PATH, ERRORS_PATH, "default.html");
}

std::string const RouterService::get_dir_content_file_path() const
{
    return build_path(PUBLIC_PATH, DIR_PATH, "index.html");
}