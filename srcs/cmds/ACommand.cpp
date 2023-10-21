#include "ACommand.hpp"

ACommand::ACommand(Server *server) : _server(server), _command(""), _args()
{
}

ACommand::~ACommand()
{
}

void ACommand::setCommand(std::vector<std::string> args)
{
    this->_command = args[1];
    setArgs(args);
}

void ACommand::setArgs(std::vector<std::string> args)
{
    this->_args = args;
}

std::string ACommand::getCommand() const
{
    return (this->_command);
}

std::vector<std::string> ACommand::getArgs()
{
    return (this->_args);
}