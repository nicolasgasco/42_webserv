#include "../utils.hpp"

#include "../../src/HttpRequest.hpp"

#include <fstream>
#include <map>

void http_request_class_tests(bool IS_DEBUG)
{
    output_suite_title("HTTP REQUEST CLASS");
    {
        std::cout << std::endl
                  << "Request line should be parsed correctly:" << std::endl;
        {
            std::cout.setstate(std::ios_base::failbit);

            // Configure HTTP request
            HttpRequest http_request;

            char *buff = http_request.get_buff();
            std::string req_line("GET / HTTP/1.1");
            int i = 0;
            for (; i < req_line.size(); ++i)
                buff[i] = req_line[i];
            buff[i] = req_line[i];

            http_request.parse_req();

            // Create reference parsed request line
            std::vector<std::string> ref;
            ref.push_back("GET");
            ref.push_back("/");
            ref.push_back("HTTP/1.1");

            std::cout.clear();
            output_test_assertion("when it's well-formed", is_strict_equal(http_request.get_req_line(), ref, IS_DEBUG));
        }
        std::cout << std::endl
                  << "Attributes should be parsed correctly:" << std::endl;
        {
            std::cout.setstate(std::ios_base::failbit);

            // Configure HTTP request
            HttpRequest http_request;

            char *buff = http_request.get_buff();

            std::string options_str("GET / HTTP/1.1\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exc\nCache-Control: max-age=0\nConnection: keep-alive\n\n\n\n\n");

            int i = 0;
            for (; i < options_str.size(); ++i)
                buff[i] = options_str[i];
            buff[i] = options_str[i];

            http_request.parse_req();

            // Create reference parsed request line
            std::map<std::string, std::string> ref;

            std::pair<std::string, std::string> key_value_pair_1("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exc");
            ref.insert(std::pair<std::string, std::string>(key_value_pair_1));
            std::pair<std::string, std::string> key_value_pair_2("Cache-Control", "max-age=0");
            ref.insert(std::pair<std::string, std::string>(key_value_pair_2));
            std::pair<std::string, std::string> key_value_pair_3("Connection", "keep-alive");
            ref.insert(std::pair<std::string, std::string>(key_value_pair_3));

            std::cout.clear();
            output_test_assertion("when they are well-formed", is_strict_equal(http_request.get_attrs(), ref, IS_DEBUG));
        }
    }
}