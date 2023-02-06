#include "../utils.hpp"

#include "../../src/utils.hpp"

#include <iostream>
#include <string>

void trim_tests(bool IS_DEBUG)
{
    output_suite_title("TRIM UTIL");
    {
        std::string ref = "bar";
        std::cout << std::endl
                  << "Returns correct value:" << std::endl;
        {
            std::string str(" bar ");

            output_test_assertion("with one space at both ends", is_strict_equal(trim(str), ref, IS_DEBUG));
        }
        {
            std::string str("  bar  ");

            output_test_assertion("with two spaces at both ends", is_strict_equal(trim(str), ref, IS_DEBUG));
        }
        {
            std::string str("          bar           ");

            output_test_assertion("with ten spaces at both ends", is_strict_equal(trim(str), ref, IS_DEBUG));
        }
        {
            std::string str("\nbar\n");

            output_test_assertion("with one newline at both ends", is_strict_equal(trim(str), ref, IS_DEBUG));
        }
        {
            std::string str("\n\rbar\n\r");

            output_test_assertion("with one newline + carriage return at both ends", is_strict_equal(trim(str), ref, IS_DEBUG));
        }
        {
            std::string str(" \n \v \r bar \n \v \r ");

            output_test_assertion("with mixed spaces at both ends", is_strict_equal(trim(str), ref, IS_DEBUG));
        }
        {
            std::string str(" \n \v \r bar foo \n \v \r ");

            output_test_assertion("with mixed spaces at both ends and spaces in the middle", is_strict_equal(trim(str), std::string("bar foo"), IS_DEBUG));
        }
    }
}