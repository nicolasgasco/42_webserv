#include "../utils.hpp"

#include "../../inc/HttpResponse.hpp"

#include <fstream>
#include <map>

void http_response_class_tests(bool IS_DEBUG)
{
    output_suite_title("HTTP RESPONSE CLASS");
    {
        {
            std::cout << std::endl
                      << "Should generate correct default headers:" << std::endl;
            {
                std::cout.setstate(std::ios_base::failbit);

                HttpRequest req;
                RouterService router;
                CgiService cgi;
                HttpResponse res(req, router, cgi);

                std::string ref;

                ref += "Date: " + get_gmt_time() + "\r\n";
                ref += "Server: Die Veteranen/1.0\r\n";
                ref += "Content-Length: 100\r\n";

                std::cout.clear();
                output_test_assertion("when it's well-formed", is_strict_equal(res.test_build_headers(100), ref, IS_DEBUG));
            }
        }
    }
}