#include "utils.hpp"
#include "Colors.hpp"

void    displayWelcome(void)
{
    std::cout << "*************************************************" << std::endl;
    std::cout << BLUE << "*           Welcome to My IRC Server!           *" << RESET << std::endl;
    std::cout << "*************************************************" << std::endl;
    std::cout << "*                                               *" << std::endl;
    std::cout << "*   To get started, please follow these         *" << std::endl;
    std::cout << "*   steps:                                      *" << std::endl;
    std::cout << "*                                               *" << std::endl;
    std::cout << "*   1. Use PASS <password> to log in            *" << std::endl;
    std::cout << "*   2. Use USER <username> 0 * :<realname>      *" << std::endl;
    std::cout << "*   3. Use NICK <nickname> to set your nickname *" << std::endl;
    std::cout << "*                                               *" << std::endl;
    std::cout << "*   To see a list of available commands,        *" << std::endl;
    std::cout << "*   type HELP.                                  *" << std::endl;
    std::cout << "*                                               *" << std::endl;
    std::cout << "*   To quit, type QUIT.                         *" << std::endl;
    std::cout << "*                                               *" << std::endl;
    std::cout << "*   To launch in DEBUG mode:                    *" << std::endl;
    std::cout << "*   ./ft_irc <port> <password> DEBUG            *" << std::endl;
    std::cout << "*                                               *" << std::endl;
    std::cout << "*************************************************" << std::endl;
}

void    printDebug(std::string const &str)
{
    printTime();
    std::cout << "[DEBUG]: " << str << std::endl;
}

void    printTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "] ";
}

std::string toString(int const &i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

bool    printable(std::string const &str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] < 32 || str[i] > 126)
            return false;
    }
    return true;
}