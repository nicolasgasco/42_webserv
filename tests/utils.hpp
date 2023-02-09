#pragma once

#include <iostream>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <map>
#include <vector>

#include "../src/HttpRequest.hpp"

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
              << YELLOW << std::uppercase << title << NC;
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
 * Checks for equality of two values of type T.
 *
 * @param value The original value.
 * @param reference The reference value.
 * @param IS_DEBUG Boolean to toggle debug output.
 */
template <typename T>
bool is_strict_equal(T const value, T const reference, bool IS_DEBUG)
{
    if (IS_DEBUG)
        std::cout << std::boolalpha << "=====own===== " << value << " | " << reference << " =====ref=====" << std::endl;
    return (value == reference);
}

/**
 * Checks for equality of two ReqLine structs.
 *
 * @param value The original ReqLine.
 * @param reference The reference ReqLine.
 * @param IS_DEBUG Boolean to toggle debug output.
 */
bool is_strict_equal(ReqLine const &value, ReqLine const &reference, bool IS_DEBUG)
{
    if (IS_DEBUG)
    {
        std::cout << std::boolalpha << "=====own===== " << std::endl;
        std::cout << "method: " << value.method << std::endl;
        std::cout << "target: " << value.target << std::endl;
        std::cout << "version: " << value.version << std::endl;

        std::cout << " =====ref=====" << std::endl;
        std::cout << "method: " << reference.method << std::endl;
        std::cout << "target: " << reference.target << std::endl;
        std::cout << "version: " << reference.version << std::endl;
    }
    if (value.method == reference.method && value.target == reference.target && value.version == reference.version)
        return true;
    return false;
}

struct Pair_First_Equal
{
    template <typename Pair>
    bool operator()(Pair const &lhs, Pair const &rhs) const
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

/**
 * Checks for equality of two maps containing keys and values of type T.
 *
 * @param value The original map.
 * @param reference The reference map.
 * @param IS_DEBUG Boolean to toggle debug output.
 */
template <typename T>
bool is_strict_equal(std::map<T, T> const &value, std::map<T, T> const &reference, bool IS_DEBUG)
{
    if (IS_DEBUG)
    {
        std::cout << std::boolalpha << "=====own===== " << std::endl;
        for (auto it = value.cbegin(); it != value.cend(); ++it)
            std::cout << it->first << ": " << it->second << std::endl;

        std::cout << " =====ref=====" << std::endl;
        for (auto it = reference.cbegin(); it != reference.cend(); ++it)
            std::cout << it->first << ": " << it->second << std::endl;
    }
    return (value.size() == reference.size() && std::equal(value.begin(), value.end(),
                                                           reference.begin(),
                                                           Pair_First_Equal()));
}

/**
 * Check if two addrinfo structs are strictly equal.
 *
 * @param info1 First addrinfo struct to compare.
 * @param info2 Second addrinfo struct to compare.
 * @param IS_DEBUG Boolean indicating if debug output is required.
 * @return Boolean
 */
bool is_strict_equal_addrinfo(const struct addrinfo info1, const struct addrinfo info2, bool IS_DEBUG)
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