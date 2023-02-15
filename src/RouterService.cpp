#include "RouterService.hpp"

RouterService::RouterService()
{
}

RouterService::~RouterService()
{
}

std::string RouterService::get_file_path(HttpRequest const &req) const
{
    std::string target = req.get_req_line().target;

    if (req.is_html_req() && target.find(".") == std::string::npos)
        return build_path(PUBLIC_PATH, target, "index.html");
    else
        return build_path(PUBLIC_PATH, target);
}

std::string RouterService::get_404_file_path() const
{
    return build_path(PUBLIC_PATH, ERRORS_PATH, "404.html");
}