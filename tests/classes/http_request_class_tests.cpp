#include "../utils.hpp"

#include "../../src/HttpRequest.hpp"

#include <fstream>
#include <map>

void http_request_class_tests(bool IS_DEBUG)
{
    output_suite_title("HTTP REQUEST CLASS");
    {
        {
            std::cout << std::endl
                      << "Request line should be parsed correctly:" << std::endl;
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();
                std::string req_line("GET / HTTP/1.1\r\n");
                int i = 0;
                for (; i < req_line.size(); ++i)
                    buff[i] = req_line[i];
                buff[i] = req_line[i];

                http_request.parse_req();

                // Create reference parsed request line
                ReqLine ref;
                ref.method = "GET";
                ref.target = "/";
                ref.version = "HTTP/1.1";

                std::cout.clear();
                output_test_assertion("when it's well-formed", is_strict_equal(http_request.get_req_line(), ref, IS_DEBUG));
            }
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();
                std::string req_line("\r\n\r\nGET / HTTP/1.1\r\n");
                int i = 0;
                for (; i < req_line.size(); ++i)
                    buff[i] = req_line[i];
                buff[i] = req_line[i];

                http_request.parse_req();

                // Create reference parsed request line
                ReqLine ref;
                ref.method = "GET";
                ref.target = "/";
                ref.version = "HTTP/1.1";

                std::cout.clear();
                output_test_assertion("by ignoring preceding empty lines", is_strict_equal(http_request.get_req_line(), ref, IS_DEBUG));
            }
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();
                std::string req_line("GET\t / \v HTTP/1.1\n");
                int i = 0;
                for (; i < req_line.size(); ++i)
                    buff[i] = req_line[i];
                buff[i] = req_line[i];

                http_request.parse_req();

                // Create reference parsed request line
                ReqLine ref;
                ref.method = "GET";
                ref.target = "/";
                ref.version = "HTTP/1.1";

                std::cout.clear();
                output_test_assertion("by supporting spaces other than SP", is_strict_equal(http_request.get_req_line(), ref, IS_DEBUG));
            }
        }
        {
            std::cout << std::endl
                      << "Request line error handling:" << std::endl;
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("DELETEE / HTTP/1.1\r\n");

                int i = 0;
                for (; i < options_str.size(); ++i)
                    buff[i] = options_str[i];
                buff[i] = options_str[i];

                http_request.parse_req();

                std::cout.clear();
                output_test_assertion("should be 501 if method is longer than longest supported one", is_strict_equal(http_request.gett_err().code, 501, IS_DEBUG));
            }
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("GET uri_that_is_way_too_long_long_long_long_long_long_long_long_long_long_long_long_long_long_long_long_long_long_long_long_long_ HTTP/1.1\r\n");

                int i = 0;
                for (; i < options_str.size(); ++i)
                    buff[i] = options_str[i];
                buff[i] = options_str[i];

                http_request.parse_req();

                std::cout.clear();
                output_test_assertion("should be 414 if method is longer than longest supported one", is_strict_equal(http_request.gett_err().code, 414, IS_DEBUG));
            }
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("/ HTTP/1.1\r\n");

                int i = 0;
                for (; i < options_str.size(); ++i)
                    buff[i] = options_str[i];
                buff[i] = options_str[i];

                http_request.parse_req();

                std::cout.clear();
                output_test_assertion("should be 400 if request line is made up of 2 chunks", is_strict_equal(http_request.gett_err().code, 400, IS_DEBUG));
            }
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("/\r\n");

                int i = 0;
                for (; i < options_str.size(); ++i)
                    buff[i] = options_str[i];
                buff[i] = options_str[i];

                http_request.parse_req();

                std::cout.clear();
                output_test_assertion("should be 400 if request line is made up of 1 chunk", is_strict_equal(http_request.gett_err().code, 400, IS_DEBUG));
            }
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("PATCH / HTTP/1.1\r\n");

                int i = 0;
                for (; i < options_str.size(); ++i)
                    buff[i] = options_str[i];
                buff[i] = options_str[i];

                http_request.parse_req();

                std::cout.clear();
                output_test_assertion("should be 501 if method is not supported", is_strict_equal(http_request.gett_err().code, 501, IS_DEBUG));
            }
        }
        {
            std::cout << std::endl
                      << "Attributes should be parsed correctly:" << std::endl;
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("GET / HTTP/1.1\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exc \r\nCache-Control: max-age=0\r\nConnection: keep-alive \r\n");

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
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("GET / HTTP/1.1\r\n\r\n\r\n\r\n\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exc\r\nCache-Control: max-age=0\r\nConnection: keep-alive\r\n");

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
                output_test_assertion("when they are preceded by empty lines", is_strict_equal(http_request.get_attrs(), ref, IS_DEBUG));
            }
        }
        {
            std::cout << std::endl
                      << "Attributes error handling:" << std::endl;
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("GET / HTTP/1.1\r\n Cache-Control : max-age=0\r\n");

                int i = 0;
                for (; i < options_str.size(); ++i)
                    buff[i] = options_str[i];
                buff[i] = options_str[i];

                http_request.parse_req();

                std::cout.clear();
                output_test_assertion("should be 400 if space before colon", is_strict_equal(http_request.gett_err().code, 400, IS_DEBUG));
            }
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("GET / HTTP/1.1\r\n Cache-Control: max-age=0\r\n");

                int i = 0;
                for (; i < options_str.size(); ++i)
                    buff[i] = options_str[i];
                buff[i] = options_str[i];

                http_request.parse_req();

                std::cout.clear();
                output_test_assertion("should be OK if space is before key and not colon", is_strict_equal(http_request.gett_err().code, -1, IS_DEBUG));
            }
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("GET / HTTP/1.1\r\n: max-age=0\r\n");

                int i = 0;
                for (; i < options_str.size(); ++i)
                    buff[i] = options_str[i];
                buff[i] = options_str[i];

                http_request.parse_req();

                std::cout.clear();
                output_test_assertion("should be 400 if key is missing", is_strict_equal(http_request.gett_err().code, 400, IS_DEBUG));
            }
            {
                std::cout.setstate(std::ios_base::failbit);

                // Configure HTTP request
                HttpRequest http_request;

                char *buff = http_request.get_buff();

                std::string options_str("GET / HTTP/1.1\r\n Cache-Control:\r\n");

                int i = 0;
                for (; i < options_str.size(); ++i)
                    buff[i] = options_str[i];
                buff[i] = options_str[i];

                http_request.parse_req();

                std::cout.clear();
                output_test_assertion("should be 400 if value is missing", is_strict_equal(http_request.gett_err().code, 400, IS_DEBUG));
            }
        }
    }
}