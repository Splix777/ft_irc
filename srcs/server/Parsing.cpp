#include "Parsing.hpp"
#include <iostream>

Parsing::Parsing()
{
}

Parsing::~Parsing()
{
}

std::vector<std::string>    Parsing::ftSplit(std::string bufferStr, std::string const &delim)
{
    std::vector<std::string> vec;

    if (bufferStr.empty())
        return (vec);
    
    std::string buff = bufferStr;

    // Splitting the buffer into words
    while (buff.find(delim) != std::string::npos)
    {
        std::string word = buff.substr(0, buff.find(delim));
        vec.push_back(word);
        buff.erase(0, buff.find(delim) + 1);
    }
    vec.push_back(buff);

    // Find and remove all \r\n from the vector
    for (size_t i = 0; i < vec.size(); i++)
    {
        if (vec[i].find("\r\n") != std::string::npos)
        {
            vec[i].erase(vec[i].find("\r\n"), 2);
        }
        if (vec[i].find("\n") != std::string::npos)
        {
            vec[i].erase(vec[i].find("\n"), 1);
        }
        if (vec[i].empty())
        {
            vec.erase(vec.begin() + i);
            i--;
        }
    }
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