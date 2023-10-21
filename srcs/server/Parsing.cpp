#include "Parsing.hpp"
#include <iostream>

Parsing::Parsing() : buff(), word(), flag(false)
{
}

Parsing::~Parsing()
{
}

void Parsing::parseClear(void)
{
    buff.clear();
    word.clear();
    flag = false;
}

// main function
std::vector<std::string>    Parsing::parseOn(std::string bufferStr)
{
    parseClear();
    std::vector<std::string> vec;
    if (bufferStr.empty())
        return (vec);
    this->buff = bufferStr;
    giveFlag(vec);

    for (size_t i = 0; i <= buff.size(); i++)
    {
        i = checkSpaceAndComma(i, vec);

        if (buff[i] == ':')
        {
            checkColon(i, vec);
            break;
        }
    }
    std::string::iterator it = (*(vec.end() - 1)).end() - 1;
    if (*it == '\r')
        (vec.end() - 1)->erase(it);

    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
        std::cout << *it << std::endl;

    return (vec);
}

void Parsing::giveFlag(std::vector<std::string> &vec)
{
    if (buff.find(":") != std::string::npos)
        (vec).push_back("1");
    else
        (vec).push_back("0");
}

int Parsing::checkSpaceAndComma(size_t i, std::vector<std::string> &vec)
{
    while (buff[i] == ' ' || buff[i] == ',')
    {
        flag = true;
        i++;
    }
    if ((!word.empty() && flag) || i == buff.size())
    {
        (vec).push_back(word);
        word.clear();
        word += buff[i];
        flag = false;
    }
    else if (word.empty() || !flag)
    {
        word += buff[i];
        flag = false;
    }
    return (i);
}

void Parsing::checkColon(size_t i, std::vector<std::string> &vec)
{

    if (!word.empty())
    {
        if (!word.empty() && (word.find(":") != std::string::npos))
        {
            word.erase(word.find(":"));
        }
        if (!word.empty())
        {
            (vec).push_back(word);
            word.clear();
        }
    }
    while (i < buff.size())
    {
        word += buff[++i];
    }
    (vec).push_back(word);
}

void Parsing::makeBuffString(char* buffStr)
{
    int i = 0;
    while (buffStr[i])
        buff += buffStr[i++];
}