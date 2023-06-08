#include "ServerConnection.hpp"

ServerConnection::ServerConnection()
{
    this->_is_chunked_first_run = true;
}

ServerConnection::~ServerConnection()
{
    this->_is_chunked_first_run = true;
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
        std::cerr << "Error: accept: " << strerror(errno) << std::endl;
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
void ServerConnection::receive_req(int const &client_fd, HttpRequest &req, Webserver *webserver, Server const *server)
{
    std::vector<char> buff(REC_BUFF_SIZE, 0);
    int bytes_received = recv(client_fd, (void *)buff.data(), REC_BUFF_SIZE, 0);

    // Resize buffer to the size of the actual bytes received
    buff.resize(bytes_received < 0 ? 0 : bytes_received);

    switch (bytes_received)
    {
    case -1:
        this->_has_err = true;
        std::cerr << "recv: error: " << strerror(errno) << std::endl;
        return;
    case 0:
        std::cerr << "recv: received 0 bytes" << std::endl;
        this->_read_done = true;
        return;
    default:
        std::cout << YELLOW << "Bytes received: " << bytes_received << NC << std::endl;
    }
    this->_bytes_received += bytes_received;

    // There is a body to read, but it is not chunked request
    if (req.has_body() && this->_is_chunked_first_run == true)
    {
        req.add_to_body(buff);

        // If read all there was to read
        if (bytes_received < REC_BUFF_SIZE)
            this->_read_done = true;
    }
    // No body or request wasn't parsed yet
    else
    {
        // Chunked request, but not first run (header already parsed)
        if (this->_is_chunked_first_run == false)
        {
            std::vector<char> unchunked_body = this->_unchunk_request_body(buff);

            req.add_to_body(unchunked_body);

            if (bytes_received < REC_BUFF_SIZE)
            {
                size_t max_body_size = server->get_max_body_size();

                std::string body_str(req.get_body().begin(), req.get_body().end());
                size_t content_length = body_str.substr(body_str.find("\r\n\r\n") + 4).size();

                if (content_length > max_body_size && req.get_req_line().method == "POST")
                {
                    std::cout << "⚠️ File to upload is bigger than 'max_body_size' " << std::endl;
                    std::cout << "Max file size allowed: " << max_body_size << std::endl;
                    std::cout << "File size to upload:   " << content_length << std::endl;

                    req.set_err(HTTP_413_CODE, HTTP_413_REASON);
                }
                this->_read_done = true;
            }
            return;
        }

        req.add_to_body(buff);
        req.parse_req(server, webserver);

        if (req.has_body())
        {
            // Check if there is Content-Length header
            try
            {
                int content_length = std::stoi(req.get_attrs().at(CONTENT_LENGTH));

                int max_body_size = server->get_max_body_size();
                if (content_length > max_body_size && req.get_req_line().method == "POST")
                {
                    std::cout << "⚠️ File to upload is bigger than 'max_body_size' " << std::endl;
                    std::cout << "Max file size allowed: " << max_body_size << std::endl;
                    std::cout << "File size to upload:   " << content_length << std::endl;

                    req.set_err(HTTP_413_CODE, HTTP_413_REASON);

                    this->_read_done = true;
                }

                // If req has body but is still smaller than REC_BUFF_SIZE
                if (content_length < REC_BUFF_SIZE && this->_bytes_received < REC_BUFF_SIZE)
                    this->_read_done = true;
                // If req has body but it cannot be read in only one go
                else
                    this->_read_done = false;
            }
            // It is chunked request
            catch (const std::out_of_range &e)
            {
                if (this->_is_chunked_first_run)
                {
                    std::vector<char> unchunked_body = this->_unchunk_request(req.get_body());

                    req.reset();
                    req.set_body(unchunked_body);
                    req.parse_req(server, webserver);
                    this->_is_chunked_first_run = false;
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
        this->_has_err = true;
        std::cerr << "Error: send: " << strerror(errno) << std::endl;
        return;
    case 0:
        std::cerr << "Error: send: sent 0 bytes: " << strerror(errno) << std::endl;
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

std::vector<char> ServerConnection::_unchunk_request(const std::vector<char> &body)
{
    std::string body_str(body.size(), 0);
    std::string tmp(body.begin(), body.end());

    // Removing header
    body_str = tmp.substr(0, tmp.find("\r\n\r\n") + 2);
    tmp = tmp.substr(tmp.find("\r\n\r\n") + 2);

    try
    {

        while (1)
        {
            // Remove first CRLF
            tmp = tmp.substr(tmp.find("\r\n") + 2);

            unsigned int chunk_size_int = 0;
            std::stringstream ss;
            ss << std::hex << tmp.substr(0, tmp.find("\r\n"));
            ss >> chunk_size_int;

            tmp = tmp.substr(tmp.find("\r\n") + 2);

            // TODO fix file bigger than buffer
            if (chunk_size_int > tmp.size())
            {
                chunk_size_int = tmp.size();
                body_str += tmp;
                tmp.clear();
                this->_read_done = false;
                break;
            }
            else
            {
                body_str += tmp.substr(0, chunk_size_int);
                tmp = tmp.substr(chunk_size_int);
                if (tmp == "\r\n0\r\n\r\n")
                {
                    this->_read_done = true;
                    break;
                }
            }
        }
        return std::vector<char>(body_str.begin(), body_str.end());
    }
    catch (const std::exception &e)
    {
        this->_read_done = true;
        return std::vector<char>();
    }
}

std::vector<char> ServerConnection::_unchunk_request_body(const std::vector<char> &body)
{
    std::cout << "unchunk_request_body" << std::endl;
    std::string result;
    std::string tmp(body.begin(), body.end());

    try
    {
        while (1)
        {
            size_t start = tmp.find("\r\n");
            size_t end = tmp.find("\r\n", start + 2);

            // Last CRLF in the buffer
            if (end == std::string::npos)
                break;

            unsigned int chunk_size_int = 0;
            std::string chunk_size_str = tmp.substr(start + 2, end - start - 2);
            std::stringstream ss;
            ss << std::hex << chunk_size_str;
            ss >> chunk_size_int;

            // Include cases where hex string wasn't valid
            if (chunk_size_int == 0)
            {
                // Copy buffer until that point and keep searching
                result += tmp.substr(0, start + 2);
                tmp = tmp.substr(start + 2);
            }
            else if (chunk_size_int > tmp.size())
            {
                result += tmp.substr(end + 2);
                break;
            }
            else
            {
                result += tmp.substr(0, start);
                result += tmp.substr(end + 2, chunk_size_int);

                tmp = tmp.substr(end + 2 + chunk_size_int);
            }
        }
    }
    catch (const std::exception &e)
    {
        return std::vector<char>(result.begin(), result.end());
    }

    return std::vector<char>(result.begin(), result.end());
}

bool ServerConnection::get_has_err() const
{
    return this->_has_err;
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
    this->_has_err = false;
    this->_sent_done = false;
    this->_read_done = false;
    this->_is_chunked_first_run = true;
}
