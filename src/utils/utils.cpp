#include "utils.hpp"

std::string ltrim(std::string &str)
{
    int start = 0;

    for (std::string::iterator it = str.begin(); it != str.end(); ++it, ++start)
    {
        if (!std::isspace(*it))
            break;
    }
    return str.substr(start);
}

std::string rtrim(std::string &str)
{
    int end = 0;

    for (std::string::reverse_iterator rit = str.rbegin(); rit != str.rend(); ++rit, ++end)
    {
        if (!std::isspace(*rit))
            break;
    }
    return str.substr(0, str.length() - end);
}

std::string trim(std::string &str)
{
    std::string l_trimmed = ltrim(str);
    std::string r_trimmed = rtrim(l_trimmed);
    return r_trimmed;
}

bool str_isspace(std::string &str)
{
    for (unsigned int i = 0; i < str.size(); i++)
    {
        if (!std::isspace(str[i]))
            return false;
    }
    return true;
}

bool str_ends_with(std::string const &value, std::string const &ending)
{
    if (ending.size() > value.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

/**
 * Builds a complete path from single sections.
 *
 * @param frag1 First fragment of path.
 * @param frag2 Second fragment of path.
 * @returns Complete path as string
 */
std::string build_path(std::string frag1, std::string frag2)
{
    std::string result = frag1;
    if (frag2.at(0) != '/')
        result += "/";
    result += frag2;
    return result;
}

/**
 * Builds a complete path from single sections.
 *
 * @param frag1 First fragment of path.
 * @param frag2 Second fragment of path.
 * @param frag3 Third fragment of path.
 * @returns Complete path as string
 */
std::string build_path(std::string frag1, std::string frag2, std::string frag3)
{
    std::string result = frag1;
    if (frag2.at(0) != '/')
        result += "/";
    result += frag2;
    if (frag3.at(0) != '/')
        result += "/";
    result += frag3;
    return result;
}