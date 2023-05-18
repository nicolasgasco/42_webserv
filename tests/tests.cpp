#include <iostream>

// #include "classes/address_info_class_tests.cpp"
// #include "classes/socket_class_tests.cpp"
// #include "classes/socket_connection_class_tests.cpp"
// #include "classes/http_request_class_tests.cpp"
// #include "classes/http_response_class_tests.cpp"
#include "utils/ltrim_tests.cpp"
#include "utils/rtrim_tests.cpp"
#include "utils/trim_tests.cpp"
#include "utils/str_ends_with_tests.cpp"
#include "utils/build_path_tests.cpp"
#include "utils/get_gmt_time_tests.cpp"
#include "utils/get_mime_type_tests.cpp"
#include "utils/replace_var_in_page_tests.cpp"
#include "utils/find_in_vec_tests.cpp"
#include "utils/trim_trailing_leading_slash_tests.cpp"

int main(int argc, char *argv[])
{
    bool IS_DEBUG = false;
    if (argc > 1)
        IS_DEBUG = (static_cast<std::string>(argv[1]) == "-d") ? true : false;

    ltrim_tests(IS_DEBUG);
    rtrim_tests(IS_DEBUG);
    trim_tests(IS_DEBUG);
    str_ends_with_tests(IS_DEBUG);
    build_path_tests(IS_DEBUG);
    get_gmt_time_tests(IS_DEBUG);
    get_mime_type_tests(IS_DEBUG);
    replace_var_in_page_tests(IS_DEBUG);
    find_in_vec_tests(IS_DEBUG);
    trim_trailing_leading_slash_tests(IS_DEBUG);

    // address_info_class_tests(IS_DEBUG);
    // socket_class_tests(IS_DEBUG);
    // socket_connection_class_tests(IS_DEBUG);
    // http_request_class_tests(IS_DEBUG);
    // http_response_class_tests(IS_DEBUG);
}