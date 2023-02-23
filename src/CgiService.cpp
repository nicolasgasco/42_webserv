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

        std::string path = "./public" + target;
        char *args[] = {const_cast<char *>("/bin/ls"), const_cast<char *>(path.c_str()), NULL};

        if (execve("/bin/ls", args, NULL))
            std::cerr << "Error: execve" << std::endl;
    }
    else
    {
        close(fds[PIPE_WRITE]);
        char buff[CGI_BUF_LEN] = {0};

        read(fds[PIPE_READ], buff, CGI_BUF_LEN);
        close(fds[PIPE_READ]);

        result = std::string(buff);
    }

    return result;
}