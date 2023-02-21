#include "../utils.hpp"

#include "../../inc/utils.hpp"

#include <iostream>
#include <string>

void get_mime_type_tests(bool IS_DEBUG)
{
    output_suite_title("MIME TYPE UTIL");
    {
        std::cout << std::endl
                  << "Returns correct value:" << std::endl;
        {
            {
                output_test_assertion("with .png file", is_strict_equal(get_mime_type("http://127.0.0.1/assets/42_logo.png"), std::string("image/x-png"), IS_DEBUG));
            }
            {
                output_test_assertion("with .jpeg file", is_strict_equal(get_mime_type("http://127.0.0.1/assets/dgerwig.jpeg?param=value"), std::string("image/jpeg"), IS_DEBUG));
            }
            {
                output_test_assertion("with .ico file", is_strict_equal(get_mime_type("http://127.0.0.1/assets/favicon.ico"), std::string("image/x-icon"), IS_DEBUG));
            }
            {
                output_test_assertion("with .svg file", is_strict_equal(get_mime_type("http://127.0.0.1/assets/graphic.svg"), std::string("image/svg+xml"), IS_DEBUG));
            }
            {
                output_test_assertion("with .bmp file", is_strict_equal(get_mime_type("http://127.0.0.1/assets/graphic.bmp"), std::string("image/x-MS-bmp"), IS_DEBUG));
            }
            {
                output_test_assertion("with .bmp file", is_strict_equal(get_mime_type("http://127.0.0.1/assets/graphic.gif"), std::string("image/gif"), IS_DEBUG));
            }
            {
                output_test_assertion("with .css file", is_strict_equal(get_mime_type("http://127.0.0.1/index.css"), std::string("text/css"), IS_DEBUG));
            }
            {
                output_test_assertion("with .js file", is_strict_equal(get_mime_type("chrome-extension://fmkadmapgofadopljbjfkapdkoienihi/build/installHook.js"), std::string("application/x-javascript"), IS_DEBUG));
            }
            {
                output_test_assertion("with unknown extension", is_strict_equal(get_mime_type("http://127.0.0.1/index.unknown"), std::string(""), IS_DEBUG));
            }
        }
    }
}
