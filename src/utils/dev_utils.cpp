#include "dev_utils.hpp"

/**
 * Outputs a string by replacing whitespaces with explicit identifiers.
 *
 * @param str String to output.
 */
void cout_explicit_whitespaces(std::string str)
{
    std::cout << GREY << "->";
    for (unsigned int i = 0; i < str.size(); i++)
    {
        std::cout << GREY;
        switch (str[i])
        {
        case ' ':
            std::cout << "SP";
            break;
        case '\r':
            std::cout << "CR";
            break;
        case '\n':
            std::cout << "LF" << std::endl;
            break;
        default:
            std::cout << NC << str[i];
        }
        std::cout << NC;
    }
    std::cout << GREY << "<-" << NC << std::endl << std::endl;
}