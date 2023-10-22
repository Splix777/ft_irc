#include "Parsing.hpp"
#include <iostream>

Parsing::Parsing()
{
}

Parsing::~Parsing()
{
}

std::vector<std::string>    Parsing::cmdSplit(std::string bufferStr)
{
    std::vector<std::string> vec;

    if (bufferStr.empty())
        return (vec);
    
    std::string buff = bufferStr;

    // Splitting the buffer into words
    while (buff.find(" ") != std::string::npos)
    {
        std::string word = buff.substr(0, buff.find(" "));
        vec.push_back(word);
        buff.erase(0, buff.find(" ") + 1);
    }
    vec.push_back(buff);

    // Removing \r\n from the last word
    vec.end()[-1].erase(vec.end()[-1].find("\r\n"), 2);

    return (vec);
}

bool Parsing::isPasswordValid(std::string const &password)
{
    if (password.length() > 8 || password.length() <= 2)
        return (false);
    for (size_t i = 0; i < password.length(); i++)
    {
        if (password[i] < 33 || password[i] > 126)
            return (false);
    }
    return (true);
}