#include "RouterService.hpp"

RouterService::RouterService()
{
}

RouterService::~RouterService()
{
}

std::string RouterService::get_file_path(HttpRequest const &req)
{
    std::string target = req.get_req_line().target;

    if (req.is_html_req() && !str_ends_with(target, ".html"))
        return build_path(PUBLIC_PATH, target, "index.html");
    else
        return build_path(PUBLIC_PATH, target);
}