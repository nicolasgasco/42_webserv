#include "../utils.hpp"

#include "../../inc/utils.hpp"

#include <iostream>
#include <string>

void find_in_vec_tests(bool IS_DEBUG)
{
    output_suite_title("FIND IN VEC UTIL");
    {
        {
            std::cout << std::endl
                      << "Returns false:" << std::endl;
            {
                std::string pattern = "bar";

                std::vector<char> vec;
                std::string str = "foo";

                for (size_t i = 0; i < str.length(); i++)
                    vec.push_back(str[i]);

                output_test_assertion("when pattern is not present", is_strict_equal(find_in_vec(pattern, vec.begin()), false, IS_DEBUG));
            }
            {
                std::string pattern = "bar";

                std::vector<char> vec;
                std::string str = "bafoo";

                for (size_t i = 0; i < str.length(); i++)
                    vec.push_back(str[i]);

                output_test_assertion("when pattern is partially present", is_strict_equal(find_in_vec(pattern, vec.begin()), false, IS_DEBUG));
            }
            {
                std::string pattern;

                std::vector<char> vec;
                std::string str = "foo ba";

                for (size_t i = 0; i < str.length(); i++)
                    vec.push_back(str[i]);

                output_test_assertion("when pattern is empty", is_strict_equal(find_in_vec(pattern, vec.begin()), false, IS_DEBUG));
            }
        }
        {
            std::cout << std::endl
                      << "Returns true:" << std::endl;
            {
                std::string pattern = "b";

                std::vector<char> vec;
                std::string str = "bfoo";

                for (size_t i = 0; i < str.length(); i++)
                    vec.push_back(str[i]);

                output_test_assertion("when pattern is one letter", is_strict_equal(find_in_vec(pattern, vec.begin()), true, IS_DEBUG));
            }
            {
                std::string pattern = "ba";

                std::vector<char> vec;
                std::string str = "bafoo";

                for (size_t i = 0; i < str.length(); i++)
                    vec.push_back(str[i]);

                output_test_assertion("when pattern is two letters", is_strict_equal(find_in_vec(pattern, vec.begin()), true, IS_DEBUG));
            }
            {
                std::string pattern = "bar";

                std::vector<char> vec;
                std::string str = "barfoo";

                for (size_t i = 0; i < str.length(); i++)
                    vec.push_back(str[i]);

                output_test_assertion("when pattern is three letters", is_strict_equal(find_in_vec(pattern, vec.begin()), true, IS_DEBUG));
            }
        }
    }
}
