#include "Who.hpp"
#include "IO.hpp"

Who::Who(Server *serv) : ACommand(serv)
{
}

Who::~Who()
{
}

void Who::exec(Client *client)
{
    if (client->getDebug())
        printDebug("Who Command Found, Executing Who Command");

    try
    {
        isValidFormat();
        checkClientLevel(client);
        whoCmd(client);
        if (client->getDebug())
            printDebug("Who Command Passed");
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " WHO :Invalid Format error !";
            break;

        case ERR_NEEDMOREPARAMS:
            msgBuf += " :Not enough parameters";
            break;

        case ERR_NOSUCHCHANNEL:
            msgBuf += " :No such channel";
            break;

        case ERR_NOTREGISTERED:
            msgBuf += " :You have not registered";
            break;

        default:
            break;
        }
        client->sendToClient(msgBuf);
    }
    _command.clear();
    _args.clear();
}

void Who::isValidFormat(void)
{
    // WHO [<channel> [<o>]]

    // Right now we only support WHO <channel>
    // In the future WHO will support WHO <mask> and WHO <mask> o

    // Check if there are 2 arguments
    if (_args.size() > 2)
        throw ERR_UNKNOWNERROR;
    if (_args.size() < 2)
        throw ERR_NEEDMOREPARAMS;
    // Check if the first argument is a channel name
    if (_args[1][0] != '#')
        throw ERR_UNKNOWNERROR;

}

void Who::checkClientLevel(Client *client)
{
    if (!(client->getMemberLevel() & REGISTERED))
        throw ERR_NOTREGISTERED;
}

void Who::whoCmd(Client *client)
{
    // WHO [<channel> [<o>]]

    // Check if the channel exists.
    if (_server->getChannelList().find(_args[1]) == _server->getChannelList().end())
        throw ERR_NOSUCHCHANNEL;
    // If the channel exists, send the list of clients in the channel to the client.
    std::map<int, Client *> clientList = _server->getChannelList()[_args[1]]->getClientList();
    for (std::map<int, Client *>::iterator it = clientList.begin(); it != clientList.end(); it++)
    {
        std::string msgBuf = std::string(":IRC 352 " + _args[1] + " " + it->second->getUsername() + " " + it->second->getHostName() + " " + inet_ntoa(_server->getAddr().sin_addr) + " " + it->second->getNickname() + " H :0 " + it->second->getRealname());
        client->sendToClient(msgBuf);
    }
    std::string endOfListMsg = std::string(":IRC 315 " + _args[1] + " :End of WHO list");
    client->sendToClient(endOfListMsg);
}