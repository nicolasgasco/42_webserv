#include "../utils.hpp"

#include "../../inc/utils.hpp"

#include <iostream>
#include <string>

void str_ends_with_tests(bool IS_DEBUG)
{
    output_suite_title("STR ENDS WITH UTIL");
    {
        {
            std::cout << std::endl
                      << "Returns true:" << std::endl;
            {
                std::string str = "bar foo baz";
                std::string ending = "baz";

                output_test_assertion("when ending is whole word at end of sentence", is_strict_equal(str_ends_with(str, ending), true, IS_DEBUG));
            }
            {
                std::string str = "bar foo baz";
                std::string ending = "z";

                output_test_assertion("when ending is one letter at end of sentence", is_strict_equal(str_ends_with(str, ending), true, IS_DEBUG));
            }
            {
                std::string str = "barfoobaz";
                std::string ending = "baz";

                output_test_assertion("when ending is substr at end of sentence", is_strict_equal(str_ends_with(str, ending), true, IS_DEBUG));
            }
            {
                std::string str = "barfoobaz";
                std::string ending = "barfoobaz";

                output_test_assertion("when ending is equal to whole sentence", is_strict_equal(str_ends_with(str, ending), true, IS_DEBUG));
            }
        }
        {
            std::cout << std::endl
                      << "Returns false:" << std::endl;
            {
                std::string str = "bar foo baz";
                std::string ending = "bad";

                output_test_assertion("when ending is whole word at end of sentence except for final letter", is_strict_equal(str_ends_with(str, ending), false, IS_DEBUG));
            }
            {
                std::string str = "bar foo baz";
                std::string ending = " ";

                output_test_assertion("when ending is one wrong letter at end of sentence", is_strict_equal(str_ends_with(str, ending), false, IS_DEBUG));
            }
            {
                std::string str = "barfoobaz";
                std::string ending = "oba";

                output_test_assertion("when ending is substr at end of sentence - 1 position", is_strict_equal(str_ends_with(str, ending), false, IS_DEBUG));
            }
            {
                std::string str = "barfoobaz";
                std::string ending = "barfoobazz";

                output_test_assertion("when ending is longer than whole sentence", is_strict_equal(str_ends_with(str, ending), false, IS_DEBUG));
            }
        }
    }
}
