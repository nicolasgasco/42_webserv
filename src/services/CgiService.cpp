#include "CgiService.hpp"

CgiService::CgiService()
{
}

CgiService::~CgiService()
{
}

std::string const CgiService::build_cgi_output(char *const *args, char *const *envp) const
{
    int fds[2];
    if (pipe(fds) == -1)
        std::cerr << "Error: pipe" << std::endl;

    pid_t pid = fork();
    if (pid == -1)
        std::cerr << "Error: fork" << std::endl;

    if (pid == 0)
    {
        if (dup2(fds[PIPE_WRITE], STDOUT_FILENO) == -1)
            std::cerr << "Error: dup2" << std::endl;
        close(fds[PIPE_READ]);
        close(fds[PIPE_WRITE]);

        if (execve(args[0], args, envp))
            std::cerr << "Error: execve: " << strerror(errno) << std::endl;
    }

    close(fds[PIPE_WRITE]);

    char buff[CGI_BUF_LEN] = {0};
    read(fds[PIPE_READ], buff, CGI_BUF_LEN);

    close(fds[PIPE_READ]);

    return std::string(buff);
}

std::vector<std::string> CgiService::build_envp(std::string path, std::string const &server_name, HttpRequest const &req) const
{
    std::vector<std::string> envp;

    std::string server_software = "SERVER_SOFTWARE=" + server_name + "/1.0";
    envp.push_back(server_software);

    try
    {
        std::string server_name = "SERVER_NAME=" + req.get_attrs().at("Host");
        envp.push_back(server_name);
    }
    catch (std::out_of_range &e)
    {
    }

    std::string gateway_interface = "GATEWAY_INTERFACE=CGI/1.1";
    envp.push_back(gateway_interface);

    std::string raw_target = req.get_req_line().raw_target;
    if (raw_target.size() && raw_target.find("?") != std::string::npos)
    {
        std::string query_string = "QUERY_STRING=" + raw_target.substr(raw_target.find("?") + 1);
        envp.push_back(query_string);
    }

    std::string server_protocol = "SERVER_PROTOCOL=" + req.get_req_line().version;
    envp.push_back(server_protocol);

    // TODO change with real server port
    std::string server_port = "SERVER_PORT=" + std::to_string(8180);
    envp.push_back(server_port);

    std::string request_method = "REQUEST_METHOD=" + req.get_req_line().method;
    envp.push_back(request_method);

    std::string path_info = "PATH_INFO=./" + path;
    envp.push_back(path_info);

    std::string script_name = "SCRIPT_NAME=" + req.get_req_line().target;
    envp.push_back(script_name);

    // Remote adress

    // Content Type
    try
    {
        std::string content_type = "CONTENT_TYPE=" + req.get_attrs().at("Content-Type");
        envp.push_back(content_type);
    }
    catch (std::out_of_range &e)
    {
    }

    // Content Length
    try
    {
        std::string content_length = "CONTENT_LENGTH=" + req.get_attrs().at("Content-Length");
        envp.push_back(content_length);
    }
    catch (std::out_of_range &e)
    {
    }

    return envp;
}

std::string const CgiService::get_cgi_executable(std::string const &path) const
{
    if (path.find(".sh") != std::string::npos)
        return SHELL_PATH;
    else if (path.find("py") != std::string::npos)
        return PYTHON3_PATH;
    else
        return std::string();
}