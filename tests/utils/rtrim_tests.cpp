#include "../utils.hpp"

#include "../../src/utils/utils.hpp"

#include <iostream>
#include <string>

void rtrim_tests(bool IS_DEBUG)
{
    output_suite_title("RTRIM UTIL");
    {
        std::string ref = "bar";
        std::cout << std::endl
                  << "Returns correct value:" << std::endl;
        {
            std::string str("bar ");

            output_test_assertion("with one space at end", is_strict_equal(rtrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("bar  ");

            output_test_assertion("with two spaces at end", is_strict_equal(rtrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("bar          ");

            output_test_assertion("with ten spaces at end", is_strict_equal(rtrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("bar\n");

            output_test_assertion("with one newline at end", is_strict_equal(rtrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("bar\n\r");

            output_test_assertion("with one newline + carriage return at end", is_strict_equal(rtrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("bar \n \v \r ");

            output_test_assertion("with mixed spaces at end", is_strict_equal(rtrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("bar foo \n \v \r ");

            output_test_assertion("with mixed spaces at end and spaces in the middle", is_strict_equal(rtrim(str), std::string("bar foo"), IS_DEBUG));
        }
    }
}