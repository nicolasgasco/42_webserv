#include "ServerConnection.hpp"

ServerConnection::ServerConnection()
{
}

ServerConnection::~ServerConnection()
{
}

int const &ServerConnection::accept_connection(int const &sock_id, addrinfo *addr_info)
{
    socklen_t addr_info_size = sizeof addr_info;
    this->_new_sock_id = accept(sock_id, (struct sockaddr *)addr_info, &addr_info_size);
    fcntl(this->_new_sock_id, F_SETFL, O_NONBLOCK);

    if (this->_new_sock_id == -1)
        std::cerr << "Error: accept: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << std::endl
                  << "New connection accepted (" << this->_new_sock_id << ")..." << NC << std::endl;

    return this->_new_sock_id;
}

void ServerConnection::receive_req(int const &client_fd, HttpRequest &req)
{

    if (req.has_body())
    {
        std::vector<char> buff(REC_BUFF_SIZE, 0);
        int bytes_received = recv(client_fd, (void *)buff.data(), REC_BUFF_SIZE, 0);
        if (bytes_received == -1)
        {
            this->_read_done = true;
            return;
        }
        else if (bytes_received == 0)
        {
            this->_read_done = true;
            return;
        }
        else
            std::cout << YELLOW << "Bytes received: " << bytes_received << NC << std::endl;
        this->_bytes_received += bytes_received;

        req.set_body(buff);

        if (bytes_received < REC_BUFF_SIZE)
        {
            this->_read_done = true;
            req.parse_post_req_body();
            req.parse_post_req_file_name(req.get_buff());
        }
    }
    else
    {
        std::vector<char> buff(REC_BUFF_SIZE, 0);
        int bytes_received = recv(client_fd, (void *)buff.data(), REC_BUFF_SIZE, 0);
        if (bytes_received == -1)
            std::cerr << "Error: recv: " << std::strerror(errno) << std::endl;
        else if (bytes_received == 0)
        {
            // TODO check what to do in this case
        }
        else
            std::cout << YELLOW << "Bytes received: " << this->_bytes_received << NC << std::endl;
        this->_bytes_received += bytes_received;

        req.set_buff(buff.data());
        req.set_body(buff);

        req.parse_req();

        this->_read_done = req.has_body() ? false : true;
    }
}

void ServerConnection::send_res(int const &client_fd, HttpResponse &res)
{
    int bytes_sent = send(client_fd, res.get_buff().c_str(), res.get_buff().length(), MSG_DONTWAIT);

    if (bytes_sent == -1)
        std::cerr << "Error: send" << std::endl;
    else if (bytes_sent == 0)
    {
        // TODO Check what to do in this case
    }
    else
        std::cout << YELLOW << "Bytes sent: " << bytes_sent << NC << std::endl;

    this->_bytes_sent += bytes_sent;

    if (res.get_buff().length() == (size_t)bytes_sent)
        this->_sent_done = true;
    else if (res.get_buff().length() > (size_t)bytes_sent)
    {
        this->_sent_done = false;

        std::string remaining_buff = res.get_buff().substr(bytes_sent);
        res.set_buff(remaining_buff);
    }
}

int const &ServerConnection::get_new_sock_id() const
{
    return this->_new_sock_id;
}

bool ServerConnection::get_send_done() const
{
    return this->_sent_done;
}

bool ServerConnection::get_read_done() const
{
    return this->_read_done;
}

void ServerConnection::reset()
{
    this->_new_sock_id = 0;
    this->_bytes_received = 0;
    this->_bytes_sent = 0;
    this->_sent_done = false;
    this->_read_done = false;
}