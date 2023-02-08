#include "../utils.hpp"

#include "../../src/utils/utils.hpp"

#include <iostream>
#include <string>

void ltrim_tests(bool IS_DEBUG)
{
    output_suite_title("LTRIM UTIL");
    {
        std::string ref = "bar";
        std::cout << std::endl
                  << "Returns correct value:" << std::endl;
        {
            std::string str(" bar");

            output_test_assertion("with one space at beginning", is_strict_equal(ltrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("  bar");

            output_test_assertion("with two spaces at beginning", is_strict_equal(ltrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("          bar");

            output_test_assertion("with ten spaces at beginning", is_strict_equal(ltrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("\nbar");

            output_test_assertion("with one newline at beginning", is_strict_equal(ltrim(str), ref, IS_DEBUG));
        }
        {
            std::string str("\n\rbar");

            output_test_assertion("with one newline + carriage return at beginning", is_strict_equal(ltrim(str), ref, IS_DEBUG));
        }
        {
            std::string str(" \n \v \r bar");

            output_test_assertion("with mixed spaces at beginning", is_strict_equal(ltrim(str), ref, IS_DEBUG));
        }
        {
            std::string str(" \n \v \r bar foo");

            output_test_assertion("with mixed spaces at beginning and spaces in the middle", is_strict_equal(ltrim(str), std::string("bar foo"), IS_DEBUG));
        }
    }
}