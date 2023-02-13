#include "../utils.hpp"

#include <iostream>
#include <string>

void build_path_tests(bool IS_DEBUG)
{
    output_suite_title("BUILD PATH UTIL");
    {
        std::string ref = "bar";
        std::cout << std::endl
                  << "Returns correct value:" << std::endl;
        {
            std::string own = build_path("frag1", "frag2");

            std::string ref = "frag1/frag2";

            output_test_assertion("with two fragments without /", is_strict_equal(own, ref, IS_DEBUG));
        }
        {
            std::string own = build_path("frag1", "frag2", "frag3");

            std::string ref = "frag1/frag2/frag3";

            output_test_assertion("with three fragments without /", is_strict_equal(own, ref, IS_DEBUG));
        }
        {
            std::string own = build_path("frag1", "/frag2");

            std::string ref = "frag1/frag2";

            output_test_assertion("with two fragments with /", is_strict_equal(own, ref, IS_DEBUG));
        }
        {
            std::string own = build_path("frag1", "/frag2", "/frag3");

            std::string ref = "frag1/frag2/frag3";

            output_test_assertion("with three fragments with /", is_strict_equal(own, ref, IS_DEBUG));
        }
        {
            std::string own = build_path("frag1", "frag2", "/frag3");

            std::string ref = "frag1/frag2/frag3";

            output_test_assertion("with three fragments and some of them have /", is_strict_equal(own, ref, IS_DEBUG));
        }
    }
}