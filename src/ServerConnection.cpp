#include "ServerConnection.hpp"

ServerConnection::ServerConnection()
{
}

ServerConnection::~ServerConnection()
{
    std::cout << YELLOW << "Connection (" << this->_new_sock_id << ") closed..." << NC << std::endl;
    close(this->_new_sock_id);
}

int const &ServerConnection::accept_connection(int const &sock_id, addrinfo *addr_info)
{
    socklen_t addr_info_size = sizeof addr_info;
    this->_new_sock_id = accept(sock_id, (struct sockaddr *)addr_info, &addr_info_size);

    if (this->_new_sock_id == -1)
        std::cerr << "Error: accept: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << std::endl
                  << "New connection accepted (" << this->_new_sock_id << ")..." << NC << std::endl;

    return this->_new_sock_id;
}

void ServerConnection::handle_connection(int const &client_fd, RouterService const &router)
{
    HttpRequest req;

    this->_receive_req(client_fd, req);

    this->_send_res(client_fd, req, router);
}

void ServerConnection::_receive_req(int const &client_fd, HttpRequest &req)
{
    char buff[REC_BUFF_SIZE];
    this->_bytes_received = recv(client_fd, (void *)buff, REC_BUFF_SIZE, 0);
    if (this->_bytes_received == -1)
        std::cerr << "Error: recv: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << "Bytes received: " << this->_bytes_received << NC << std::endl;

    req.set_buff(buff);

    req.parse_req();
    req.output_status();
}

void ServerConnection::_send_res(int const &client_fd, HttpRequest &req, RouterService const &router)
{
    HttpResponse res(req, router);

    this->_bytes_sent = send(client_fd, res.get_buff().c_str(), res.get_buff().length(), 0);
    if (this->_bytes_sent == -1)
        std::cerr << "Error: send: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << "Bytes sent: " << this->_bytes_sent << NC << std::endl;
}

int const &ServerConnection::get_new_sock_id() const
{
    return this->_new_sock_id;
}