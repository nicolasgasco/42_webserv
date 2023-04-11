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

/*    std::string s = _req_line.target;
    std::string *path_com = NULL;
    std::string path;
    path_com = &path;
    int init = 0;
    int end = 0;
    while (end = s.find("/", init), end >= 0)
    {
        path = s.substr(init, end - init);
        if (path.length() > 0)
            path = "/" + path;
        init = end + 1;
    }
    std::cout << std::endl;*/

	std::string index_page = "index.html";
	std::cout << "🔴 INDEX PAGE  initial: " << index_page << std::endl;
	
	std::string path_to_find = req.get_req_line().target;
	std::cout << "🟢🟢🟢 PATH to find: " << path_to_find << std::endl;

	std::vector<Location> location = server->get_location_blocks();
    for (std::vector<Location>::iterator it = location.begin(); it != location.end(); it++)
 	{
		Location location = *it;
        std::cout << "🟠🟠🟠 location -> " << location.get_location() << std::endl;
        std::string index_redir = location.get_index();
		std::cout << "🔵 INDEX TO REDIR: " << " -> " << index_redir << std::endl;
		if (path_to_find == location.get_location())
		{
			index_page = index_redir;
			std::cout << "🔴 INDEX PAGE  changing: " << index_page << std::endl;
			break;
		}
	}
	std::cout << "🔴 INDEX PAGE  final: " << index_page << std::endl;

    std::string target = req.get_req_line().target;

    bool does_contain_dot = target.find(".") != std::string::npos;

    if (req.is_cgi_req())
        // cgi-bin is at the root of the server
        return build_path("./", target);
    // If it contains a dot, it means it has an extension and doesn't need to append index.html
    else if (req.is_html_req() && !does_contain_dot)
        //return build_path(PUBLIC_PATH, target, "index.html");
        return build_path(PUBLIC_PATH, target, index_page);
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
	std::cout << "🔴 GET_DIR_CONTENT_FILE_PATH: " << std::endl;
    return build_path(PUBLIC_PATH, DIR_PATH, "index.html");
}
