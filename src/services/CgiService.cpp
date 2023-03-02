#include "CgiService.hpp"

CgiService::CgiService()
{
}

CgiService::~CgiService()
{
}

std::string const CgiService::build_dir_content(std::string const &target) const
{
    char *args[] = {const_cast<char *>("/usr/bin/python3"), const_cast<char *>("public/cgi_bin/output_dir_content.py"), NULL};

    std::string path = "PATH_INFO=./public" + target;
    char *envp[] = {const_cast<char *>(path.c_str()), NULL};

    return this->build_cgi_output(args, envp);
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