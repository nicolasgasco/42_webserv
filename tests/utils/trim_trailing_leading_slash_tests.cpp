#include "../utils.hpp"

#include "../../inc/utils.hpp"

#include <iostream>
#include <string>

void trim_trailing_leading_slash_tests(bool IS_DEBUG)
{
    output_suite_title("TRIM TRAILING AND LEADING SLASH UTIL");
    {
        std::string ref = "bar";
        std::cout << std::endl
                  << "Returns correct value:" << std::endl;
        {
            std::string str("/bar");

            output_test_assertion("with trailing slash", is_strict_equal(trim_trailing_leading_slash(str), ref, IS_DEBUG));
        }
        {
            std::string str("bar/");

            output_test_assertion("with leading slash", is_strict_equal(trim_trailing_leading_slash(str), ref, IS_DEBUG));
        }
        {
            std::string str("/bar/");

            output_test_assertion("with trailing and leading slash", is_strict_equal(trim_trailing_leading_slash(str), ref, IS_DEBUG));
        }
        {
            std::string str("bar");

            output_test_assertion("without trailing and leading slash", is_strict_equal(trim_trailing_leading_slash(str), ref, IS_DEBUG));
        }
    }
}
