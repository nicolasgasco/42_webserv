#include "ServerConnection.hpp"

ServerConnection::ServerConnection()
{
}

ServerConnection::~ServerConnection()
{
    std::cout << YELLOW << "Connection (" << this->_new_sock_id << ") closed..." << NC << std::endl;
    close(this->_new_sock_id);
}

void ServerConnection::accept_connection(int const &sock_id, addrinfo *addr_info)
{
    socklen_t addr_info_size = sizeof addr_info;
    this->_new_sock_id = accept(sock_id, (struct sockaddr *)addr_info, &addr_info_size);

    if (this->_new_sock_id == -1)
        std::cerr << "Error: accept: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << std::endl
                  << "New connection accepted (" << this->_new_sock_id << ")..." << NC << std::endl;
}

void ServerConnection::handle_connection(RouterService const &router)
{
    HttpRequest req;

    this->_receive_req(req);

    this->_send_res(req, router);
}

void ServerConnection::_receive_req(HttpRequest &req)
{
    char buff[REC_BUFF_SIZE];
    this->_bytes_received = recv(this->_new_sock_id, (void *)buff, REC_BUFF_SIZE, 0);
    if (this->_bytes_received == -1)
        std::cerr << "Error: recv: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << "Bytes received: " << this->_bytes_received << NC << std::endl;

    req.set_buff(buff);

    req.parse_req();
    req.output_status();
}

void ServerConnection::_send_res(HttpRequest &req, RouterService const &router)
{
    HttpResponse res(req, router);

    this->_bytes_sent = send(this->_new_sock_id, res.get_buff().c_str(), res.get_buff().length(), 0);
    if (this->_bytes_sent == -1)
        std::cerr << "Error: send: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << "Bytes sent: " << this->_bytes_sent << NC << std::endl;
}

int const &ServerConnection::get_new_sock_id() const
{
    return this->_new_sock_id;
}