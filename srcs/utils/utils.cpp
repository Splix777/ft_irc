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
    std::cout << "*                    _                          *" << std::endl;  
    std::cout << "*                  -=\\`\\                        *" << std::endl;  
    std::cout << "*              |\\ ____\\_\\__                     *" << std::endl;  
    std::cout << "*            -=\\c`'''''''''''`)                 *" << std::endl;  
    std::cout << "*               `~~~~~/ /~~`~~                  *" << std::endl;  
    std::cout << "*                 -==/ /                        *" << std::endl;  
    std::cout << "*                   '-'                         *" << std::endl;  
    std::cout << "*                  _  _                         *" << std::endl;  
    std::cout << "*                 ( `   )_                      *" << std::endl;  
    std::cout << "*                (    )    `)                   *" << std::endl;  
    std::cout << "*              (_   (_ .  _) _)                 *" << std::endl;  
    std::cout << "*                                               *" << std::endl;  
    std::cout << "*                                (  )           *" << std::endl;  
    std::cout << "*             _ .             ( `  ) . )        *" << std::endl;  
    std::cout << "*           (  _ )_          (_, _(  ,_)_)      *" << std::endl;  
    std::cout << "*         (_  _(_ ,)                            *" << std::endl;  
    std::cout << "*************************************************" << std::endl;
}

void    displayHelp()
{}

bool    isPasswordValid(std::string const &password)
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