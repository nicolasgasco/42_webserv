#include "CgiService.hpp"
#include <fcntl.h>

CgiService::CgiService()
{
}

CgiService::~CgiService()
{
}

std::string const CgiService::build_cgi_output(char *const *args, char *const *envp, const std::vector<char> *req_body) const
{
    std::string cgi_output;
    std::vector<char> body;
    if (req_body)
        body = *req_body;

    bool finished_writing = false;
    while (!finished_writing)
    {
        int fds[2][2];

        if (req_body)
        {
            if (pipe(fds[PIPE_BODY]) == -1)
                std::cerr << "Error: pipe: " << strerror(errno) << std::endl;

            fcntl(fds[PIPE_BODY][FD_WRITE], F_SETFL, O_NONBLOCK);
            ssize_t bytes_written = write(fds[PIPE_BODY][FD_WRITE], body.data(), body.size() * sizeof(char));
            close(fds[PIPE_BODY][FD_WRITE]);

            if (bytes_written < 0)
            {
                std::cerr << "Error: write: " << strerror(errno) << std::endl;
                finished_writing = true;
            }
            else if (bytes_written < (ssize_t)body.size())
                body.erase(body.begin(), body.begin() + bytes_written);
            else
                finished_writing = true;
        }
        else
            finished_writing = true;

        if (pipe(fds[PIPE_RES]) == -1)
            std::cerr << "Error: pipe: " << strerror(errno) << std::endl;

        pid_t pid = fork();
        if (pid == -1)
            std::cerr << "Error: fork: " << strerror(errno) << std::endl;
        else if (pid == 0)
        {
            if (dup2(fds[PIPE_RES][FD_WRITE], STDOUT_FILENO) == -1)
                std::cerr << "Error: dup2: " << strerror(errno) << std::endl;
            close(fds[PIPE_RES][FD_READ]);
            close(fds[PIPE_RES][FD_WRITE]);

            if (req_body)
            {
                if (dup2(fds[PIPE_BODY][FD_READ], STDIN_FILENO) == -1)
                    std::cerr << "Error: dup2: " << strerror(errno) << std::endl;
                close(fds[PIPE_BODY][FD_READ]);
                close(fds[PIPE_BODY][FD_WRITE]);
            }

            if (execve(args[0], args, envp) < 0)
                std::cerr << "Error: execve: " << strerror(errno) << std::endl;
        }
        else
        {
            if (req_body)
                close(fds[PIPE_BODY][FD_READ]);

            close(fds[PIPE_RES][FD_WRITE]);

            char buff[CGI_BUF_LEN] = {0};
            read(fds[PIPE_RES][FD_READ], buff, CGI_BUF_LEN);
            close(fds[PIPE_RES][FD_READ]);

            if (finished_writing)
                cgi_output = buff;
        }
    }
    return cgi_output;
}

std::string const CgiService::build_cgi_output(char *const *args, char *const *envp) const
{
    std::string cgi_output;

    int fds[2];

    if (pipe(fds) == -1)
    {
        std::cerr << "Error: pipe: " << strerror(errno) << std::endl;
        // TODO Add 500 error page
        return cgi_output;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "Error: fork: " << strerror(errno) << std::endl;
        // TODO Add 500 error page
        return cgi_output;
    }

    if (pid == 0)
    {
        if (dup2(fds[FD_WRITE], STDOUT_FILENO) == -1)
        {
            std::cerr << "Error: dup2: " << strerror(errno) << std::endl;
            // TODO Add 500 error page
            return cgi_output;
        }
        close(fds[FD_READ]);
        close(fds[FD_WRITE]);

        if (execve(args[0], args, envp) < 0)
        {
            std::cerr << "Error: execve: " << strerror(errno) << std::endl;
            // TODO Add 500 error page
            return cgi_output;
        }
    }
    else
    {
        close(fds[FD_WRITE]);

        char buff[CGI_BUF_LEN] = {0};
        read(fds[FD_READ], buff, CGI_BUF_LEN);
        close(fds[FD_READ]);

        cgi_output = buff;
    }

    return cgi_output;
}

std::vector<std::string> CgiService::build_envp(std::string path, Server const *server, HttpRequest const &req) const
{
    std::vector<std::string> envp;

    // Content Length
    try
    {
        std::string content_length = "CONTENT_LENGTH=" + req.get_attrs().at("Content-Length");
        envp.push_back(content_length);
    }
    catch (std::out_of_range &e)
    {
    }

    // Content Type
    try
    {
        std::string content_type = "CONTENT_TYPE=" + req.get_attrs().at("Content-Type");
        envp.push_back(content_type);
    }
    catch (std::out_of_range &e)
    {
    }

    std::string gateway_interface = "GATEWAY_INTERFACE=CGI/1.1";
    envp.push_back(gateway_interface);

    std::string path_info = "PATH_INFO=" + path;
    envp.push_back(path_info);

    std::string path_translated = "PATH_TRANSLATED=./" + path;
    envp.push_back(path_translated);

    std::string raw_target = req.get_req_line().raw_target;
    if (raw_target.size() && raw_target.find("?") != std::string::npos)
    {
        std::string query_string = "QUERY_STRING=" + raw_target.substr(raw_target.find("?") + 1);
        envp.push_back(query_string);
    }

    // Remote adress
    // TODO change this to the real remote address
    std::string remote_address = "REMOTE_ADDR=127.0.0.0";
    envp.push_back(remote_address);

    std::string request_method = "REQUEST_METHOD=" + req.get_req_line().method;
    envp.push_back(request_method);

    std::string script_name = "SCRIPT_NAME=" + req.get_req_line().target;
    envp.push_back(script_name);
    try
    {
        std::string server_name = "SERVER_NAME=" + req.get_attrs().at("Host");
        if (server_name.size() && server_name.find(":") != std::string::npos)
            server_name = server_name.substr(0, server_name.find(":"));
        envp.push_back(server_name);
    }
    catch (std::out_of_range &e)
    {
    }

    std::string server_port = "SERVER_PORT=" + server->get_port();
    envp.push_back(server_port);

    std::string server_protocol = "SERVER_PROTOCOL=" + req.get_req_line().version;
    envp.push_back(server_protocol);

    std::string server_software = "SERVER_SOFTWARE=" + server->get_server_name() + "/1.0";
    envp.push_back(server_software);

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