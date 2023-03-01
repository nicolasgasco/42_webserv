#include "utils.hpp"

std::string ltrim(std::string const &str)
{
    int start = 0;

    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it, ++start)
    {
        if (!std::isspace(*it))
            break;
    }
    return str.substr(start);
}

std::string rtrim(std::string const &str)
{
    int end = 0;

    for (std::string::const_reverse_iterator rit = str.rbegin(); rit != str.rend(); ++rit, ++end)
    {
        if (!std::isspace(*rit))
            break;
    }
    return str.substr(0, str.length() - end);
}

std::string trim(std::string const &str)
{
    std::string l_trimmed = ltrim(str);
    std::string r_trimmed = rtrim(l_trimmed);
    return r_trimmed;
}

bool str_isspace(std::string const &str)
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
std::string build_path(std::string const &frag1, std::string const &frag2)
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
std::string build_path(std::string const &frag1, std::string const &frag2, std::string const &frag3)
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

/**
 * Returns a string with current GMT time.
 *
 * @returns Current GMT time formatted for HTTP header.
 */
std::string const get_gmt_time()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[100];

    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    // E.g. Wed, 21 Oct 2015 07:28:00 GMT
    strftime(buffer, sizeof(buffer), "%a, %d %b %G %T GMT", timeinfo);

    return std::string(buffer);
}

/**
 * Returns the MIME type of a file from a URI.
 *
 * @param target Request URI.
 * @returns MIME type extract for target or NULL.
 */
std::string const get_mime_type(std::string target)
{
    std::string result;

    // Images
    if (target.find(".png") != std::string::npos)
        return "image/x-png";
    else if (target.find(".jpeg") != std::string::npos)
        return "image/jpeg";
    else if (target.find(".ico") != std::string::npos)
        return "image/x-icon";
    else if (target.find(".svg") != std::string::npos)
        return "image/svg+xml";
    else if (target.find(".bmp") != std::string::npos)
        return "image/x-MS-bmp";
    else if (target.find(".gif") != std::string::npos)
        return "image/gif";

    if (target.find(".css") != std::string::npos)
        return "text/css";

    if (target.find(".js") != std::string::npos)
        return "application/x-javascript";

    return result;
}

/**
 * Scans for template variable and replace it with value.
 *
 * @param file File where the variable is.
 * @param var Name of the variable.
 * @param value Value to be injected.
 */
void replace_var_in_page(std::string &file, std::string const var, std::string const value)
{
    while (true)
    {
        size_t posCode = file.find(var);
        if (posCode == std::string::npos)
            return;

        file.erase(posCode, var.length());
        file.insert(posCode, value);
    }
}

/**
 * Returns true if a specified pattern is found in a vector of chars.
 *
 * @param pattern String representing the pattern to be searched.
 * @param it Iterator representing the position from where the search start
 * @returns True if pattern is found, false otherwise.
 */
bool find_in_vec(std::string const &pattern, std::vector<char>::iterator it)
{
    if (pattern.length() == 0)
        return false;

    for (size_t i = 0; i < pattern.length(); i++)
    {
        if (*it != pattern[i])
            return false;
        it++;
    }
    return true;
}