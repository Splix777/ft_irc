#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Channel.hpp"
# include "Client.hpp"

# include <cstdlib>
# include <exception>
# include <iostream>
# include <map>
# include <sstream>
# include <string>
# include <vector>


class Server;

class ACommand
{
    protected:
        Server *_server;
        std::string _command;
        std::vector<std::string> _args;

        ACommand();
    public:
        ACommand(Server *server);
        virtual ~ACommand();

        void setCommand(std::vector<std::string> args);
        void setArgs(std::vector<std::string> args);

        std::string					getCommand() const;
        std::vector<std::string>    getArgs();

        virtual void exec(Client *client) = 0;
};

#endif