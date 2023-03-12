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

std::vector<std::string> CgiService::build_envp(std::string path, HttpRequest const &req) const
{
    std::vector<std::string> envp;

    std::string server_protocol = "SERVER_PROTOCOL=" + req.get_req_line().version;
    envp.push_back(server_protocol);

    // TODO change with real server port
    std::string server_port = "SERVER_PORT=" + std::to_string(8180);
    envp.push_back(server_port);

    std::string request_method = "REQUEST_METHOD=" + req.get_req_line().method;
    envp.push_back(request_method);

    std::string path_info = "PATH_INFO=./" + path;
    envp.push_back(path_info);

    try
    {
        if (req.get_attrs().at("Host").length())
        {
            std::string remote_host = "REMOTE_HOST=" + req.get_attrs().at("Host");
            envp.push_back(const_cast<char *>(remote_host.c_str()));
        }
    }
    catch (std::out_of_range &e)
    {
    }

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