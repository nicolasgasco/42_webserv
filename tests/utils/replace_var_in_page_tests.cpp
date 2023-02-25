#include "../utils.hpp"

#include <iostream>
#include <string>

void replace_var_in_page_tests(bool IS_DEBUG)
{
    output_suite_title("REPLACE VAR IN PAGE:");
    {
        std::cout << std::endl
                  << "Returns correct value:" << std::endl;
        {
            std::string own = "<p>{{var}}</p>";
            replace_var_in_page(own, "{{var}}", "value");

            std::string ref = "<p>value</p>";

            output_test_assertion("with one variable", is_strict_equal(own, ref, IS_DEBUG));
        }
        {
            std::string own = "<p>{{var}} is {{var}}</p>";
            replace_var_in_page(own, "{{var}}", "value");

            std::string ref = "<p>value is value</p>";

            output_test_assertion("with two variables", is_strict_equal(own, ref, IS_DEBUG));
        }
        {
            std::string own = "<p>{{var}} is {{var}} is {{var}}</p>";
            replace_var_in_page(own, "{{var}}", "value");

            std::string ref = "<p>value is value is value</p>";

            output_test_assertion("with three variables", is_strict_equal(own, ref, IS_DEBUG));
        }
    }
}