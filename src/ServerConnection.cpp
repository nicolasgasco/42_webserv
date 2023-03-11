#include "ServerConnection.hpp"

ServerConnection::ServerConnection()
{
}

ServerConnection::~ServerConnection()
{
}

/**
 * Accept new connection for socket.
 *
 * @param sock_id Fd or socket.
 * @param addr_info Struct with connection info.
 * @returns Fd of new connection.
 */
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

/**
 * Reads from socket when a request is received.
 * Request can be with body (e.g. POST) or without a body.
 *
 * @param client_fd Fd where the request is sent.
 * @param req Req object where the request is stored.
 */
void ServerConnection::receive_req(int const &client_fd, HttpRequest &req, class Webserver *webserver)
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
            // TODO check that bytes_received is < max body size
            // If not, set error to 413, "Content Too Large"

            req.parse_post_req_body();
            this->_read_done = true;
        }
    }
    // No body or request wasn't parsed yet
    else
    {
        req.set_body(buff);

        req.parse_req();

        if (req.has_body())
        {


            // Check if there is Content-Length header
            try
            {
                int content_length = std::stoi(req.get_attrs().at(CONTENT_LENGTH));

                // TODO check here if content_length is bigger than max body size
                // If so, this->_set_err(413, "Content Too Large"); + set this->_read_done as true

                // If req has body but is still smaller than REC_BUFF_SIZE
                if (content_length < REC_BUFF_SIZE && this->_bytes_received < REC_BUFF_SIZE)
                {
                    req.parse_post_req_body();
                    this->_read_done = true;
                }
                // If req has body but it cannot be read in only one go
                else
                    this->_read_done = false;
            }
            // It is chunked request
            catch (const std::out_of_range &e)

            {
                // If end of body was found already
                std::string search_pattern = "0\r\n\r\n";
                for (std::vector<char>::const_iterator it = req.get_body().begin(); it != (req.get_body().end() - search_pattern.length()); ++it)
                {
                    if (find_in_vec(search_pattern, it))
                    {
                        req.parse_post_req_body();
                        this->_read_done = true;
                        break;
                    }
                }
            }
        }
        // If req has no body
        else
            this->_read_done = true;
    }
}

/**
 * Sends a response to socket.
 *
 * @param client_fd Fd where the request is sent.
 * @param res Res object where the response is stored.
 */
void ServerConnection::send_res(int const &client_fd, HttpResponse &res)
{
    int bytes_sent = send(client_fd, res.get_buff().c_str(), res.get_buff().length(), MSG_DONTWAIT);

    switch (bytes_sent)
    {
    case -1:
        std::cerr << "Error: send: " << strerror(errno) << std::endl;
        break;
    case 0:
        std::cerr << "Error: send: " << strerror(errno) << std::endl;
        break;
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

/**
 * Resets state of connection.
 * Connections are long-lived in main.
 */
void ServerConnection::reset()
{
    this->_new_sock_id = 0;
    this->_bytes_received = 0;
    this->_bytes_sent = 0;
    this->_sent_done = false;
    this->_read_done = false;
}
