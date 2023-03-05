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

    switch (this->_new_sock_id)
    {
    case -1:
        std::cerr << "Error: accept: " << std::strerror(errno) << std::endl;
        break;
    default:
        std::cout << YELLOW << std::endl
                  << "New connection accepted (" << this->_new_sock_id << ")..." << NC << std::endl;
    }

    return this->_new_sock_id;
}

void ServerConnection::receive_req(int const &client_fd, HttpRequest &req)
{
    std::vector<char> buff(REC_BUFF_SIZE, 0);
    int bytes_received = recv(client_fd, (void *)buff.data(), REC_BUFF_SIZE, 0);

    switch (bytes_received)
    {
    case -1:
        std::cerr << "Error: recv: " << std::strerror(errno) << std::endl;
        break;
    case 0:
        this->_read_done = true;
        return;
    default:
        std::cout << YELLOW << "Bytes received: " << bytes_received << NC << std::endl;
    }
    this->_bytes_received += bytes_received;

    // There is a body to read
    if (req.has_body())
    {
        req.set_body(buff);

        // If read all there was to read
        if (bytes_received < REC_BUFF_SIZE)
        {
            this->_read_done = true;
            req.parse_post_req_file_name(req.get_body());
            req.parse_post_req_body();
        }
    }
    // No body or request wasn't parsed yet
    else
    {
        req.set_body(buff);

        req.parse_req();

        if (req.has_body())
        {
            int content_length = std::stoi(req.get_attrs().at("Content-Length"));
            // If req has body but is still smaller than REC_BUFF_SIZE
            if (content_length < REC_BUFF_SIZE && this->_bytes_received < REC_BUFF_SIZE)
            {
                req.parse_post_req_file_name(req.get_body());
                req.parse_post_req_body();
                this->_read_done = true;
            }
            // If req has body but it cannot be read in only one go
            else
                this->_read_done = false;
        }
        // If req has no body
        else
            this->_read_done = true;
    }
}

void ServerConnection::send_res(int const &client_fd, HttpResponse &res)
{
    int bytes_sent = send(client_fd, res.get_buff().c_str(), res.get_buff().length(), MSG_DONTWAIT);

    switch (bytes_sent)
    {
    case -1:
        std::cerr << "Error: send: " << strerror(errno) << std::endl;
        break;
    case 0:
        std::cerr << "porca madonna" << std::endl;
    default:
        std::cout << YELLOW << "Bytes sent: " << bytes_sent << NC << std::endl;
    }
    this->_bytes_sent += bytes_sent;

    // If sent all buffer, sent is done
    if (static_cast<size_t>(bytes_sent) == res.get_buff().length())
        this->_sent_done = true;
    // Else, send rest of buffer
    else
    {
        std::string remaining_buff = res.get_buff().substr(bytes_sent);
        res.set_buff(remaining_buff);

        this->_sent_done = false;
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