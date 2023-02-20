#include "../utils.hpp"

#include "../../inc/utils.hpp"

#include <iostream>
#include <string>

void get_gmt_time_tests(bool IS_DEBUG)
{
    output_suite_title("GET GMT TIME UTIL");
    {
        std::string ref = "bar";
        std::cout << std::endl
                  << "Returns correct value:" << std::endl;
        {

            time_t rawtime;
            struct tm *timeinfo;
            char buffer[100];
            time(&rawtime);
            timeinfo = gmtime(&rawtime);
            strftime(buffer, sizeof(buffer), "%a, %d %b %G %T GMT", timeinfo);

            output_test_assertion("with one space at both ends", is_strict_equal(get_gmt_time(), std::string(buffer), IS_DEBUG));
        }
    }
}
