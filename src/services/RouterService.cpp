#include "RouterService.hpp"

RouterService::RouterService()
{
}

RouterService::~RouterService()
{
}

/**
 * Calculates the correct path for a specific request target.
 */
std::string const RouterService::get_file_path(HttpRequest const &req, Server *server) const
{

    std::string target = req.get_req_line().target;

    if (req.is_cgi_req())
        // cgi-bin is at the root of the target
        return target.front() == '/' ? "." + target : "./" + target;
    // If index page needs to be appended to the target, compute it
    else if (req.is_dir_req() && server->get_autoindex() == true)
    {
        std::vector<Location> locations = server->get_location_blocks();
        for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
        {
            std::string index_page = it->get_index();

            std::string location = trim_trailing_leading_slash(it->get_location());
            bool is_location_in_target = static_cast<bool>(req.get_req_line().target.find(location) != std::string::npos);
            if (is_location_in_target && index_page.size() > 0)
                return build_path(PUBLIC_PATH, target, index_page);
        }
        return build_path(PUBLIC_PATH, target, DEFAULT_INDEX_PAGE);
    }
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