#include <iostream>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

#define YELLOW "\033[0;33m"
#define NC "\033[0m"

/**
 * Outputs the title of a test suite.
 *
 * @param title Name of the test suite.
 */
void output_suite_title(std::string title)
{
    std::cout << std::endl
              << YELLOW << std::uppercase << title << NC << std::endl;
}

/**
 * Outputs the result of a test assertion.
 *
 * @param description Description of the test assertion.
 * @param result Boolean result of a test assertion.
 */
void output_test_assertion(std::string description, bool result)
{
    bool isErr = result != true;
    std::cout << "  - " << description << " " << (isErr ? "❌" : "✅") << std::endl;
    if (isErr)
        exit(1);
}

/**
 * Check if two addrinfo structs are strictly equal.
 *
 * @param info1 First addrinfo struct to compare.
 * @param info2 Second addrinfo struct to compare.
 * @param IS_DEBUG Boolean indicating if debug output is required.
 * @return Boolean
 */
bool is_strict_equal(const struct addrinfo info1, const struct addrinfo info2, bool IS_DEBUG)
{

    // addrinfo
    if (info1.ai_flags != info2.ai_flags)
    {
        std::cerr << "ai_flag not equal" << std::endl;
        return false;
    }
    if (IS_DEBUG)
        std::cout << std::endl
                  << "ai_flag: " << info1.ai_flags << ", " << info2.ai_flags << std::endl;

    if (info1.ai_family != info2.ai_family)
    {
        std::cerr << "ai_family not equal" << std::endl;
        return false;
    }
    if (IS_DEBUG)
        std::cout << "ai_family: " << info1.ai_family << ", " << info2.ai_family << std::endl;

    if (info1.ai_socktype != info2.ai_socktype)
    {
        std::cerr << "ai_socktype not equal" << std::endl;
        return false;
    }
    if (IS_DEBUG)
        std::cout << "ai_socktype: " << info1.ai_socktype << ", " << info2.ai_socktype << std::endl;

    if (info1.ai_protocol != info2.ai_protocol)
    {
        std::cerr << "ai_protocol not equal" << std::endl;
        return false;
    }
    if (IS_DEBUG)
        std::cout << "ai_protocol: " << info1.ai_protocol << ", " << info2.ai_protocol << std::endl;

    if (info1.ai_addrlen != info2.ai_addrlen)
    {
        std::cerr << "ai_addrlen not equal" << std::endl;
        return false;
    }
    if (IS_DEBUG)
        std::cout << "ai_addrlen: " << info1.ai_addrlen << ", " << info2.ai_addrlen << std::endl;

    // TODO check if this is required (not NULL)
    // if (strcmp(info1.ai_canonname, info2.ai_canonname))
    //     return false;

    // sockaddr
    if (info1.ai_addr->sa_family != info2.ai_addr->sa_family)
    {
        std::cerr << "ai_addr->sa_family not equal" << std::endl;
        return false;
    }
    if (IS_DEBUG && info1.ai_addr->sa_family && info2.ai_addr->sa_family)
        std::cout << "ai_addr->sa_family: " << info1.ai_addr->sa_family << ", " << info2.ai_addr->sa_family << std::endl
                  << std::endl;

    if (strcmp(info1.ai_addr->sa_data, info2.ai_addr->sa_data))
    {
        std::cerr << "ai_addr->sa_data not equal" << std::endl;
        return false;
    }
    // if (IS_DEBUG)
    //     std::cout << "ai_addr->sa_data: " << info1.ai_addr->sa_data << ", " << info2.ai_addr->sa_data << std::endl;

    return true;
}