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