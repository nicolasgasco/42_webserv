#include "RouterService.hpp"

RouterService::RouterService(HttpRequest &req) : _req(req)
{
}

RouterService::~RouterService()
{
}

std::string RouterService::get_file_path()
{
    std::string target = this->_req.get_req_line().target;
    bool is_target_root = target == "/";
    std::string file_path = is_target_root ? build_path(PUBLIC_PATH, "index.html") : build_path(PUBLIC_PATH, target);

    return file_path;
}