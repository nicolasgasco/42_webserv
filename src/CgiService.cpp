#include "CgiService.hpp"

CgiService::CgiService()
{
}

CgiService::~CgiService()
{
}

std::string const CgiService::build_dir_content(std::string const &target) const
{
    std::string result;

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

        char *args[] = {const_cast<char *>("/usr/bin/python3"), const_cast<char *>("./cgi_bin/output_dir_content.py"), NULL};

        std::string path = "PATH_INFO=./public" + target;
        char *envp[] = {const_cast<char *>(path.c_str()), NULL};

        if (execve("/usr/bin/python3", args, envp))
            std::cerr << "Error: execve: " << strerror(errno) << std::endl;
    }

    close(fds[PIPE_WRITE]);

    char buff[CGI_BUF_LEN] = {0};
    read(fds[PIPE_READ], buff, CGI_BUF_LEN);

    close(fds[PIPE_READ]);

    return std::string(buff);
}