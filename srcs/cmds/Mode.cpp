#include "Mode.hpp"
#include "IO.hpp"

Mode::Mode(Server *serv) : ACommand(serv)
{
}

Mode::~Mode()
{
}

void Mode::exec(Client *client)
{
    if (client->getDebug())
        printDebug("Mode Command Found, Executing Mode Command");

    try
    {
        isValidFormat();
        checkClientLevel(client);
        modeCmd(client);
        if (client->getDebug())
            printDebug("Mode Command Passed");
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " Mode :Invalid Format error !";
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

void Mode::isValidFormat(void)
{
    // Mode [<channel> [<o>]]

    // Right now we only support Mode <channel>
    // In the future Mode will support Mode <mask> and Mode <mask> o

    // Check if there are 2 arguments
    if (_args.size() > 2)
        throw ERR_UNKNOWNERROR;
    if (_args.size() < 2)
        throw ERR_NEEDMOREPARAMS;
    // Check if the first argument is a channel name
    if (_args[1][0] != '#')
        throw ERR_UNKNOWNERROR;

}

void Mode::checkClientLevel(Client *client)
{
    if (!(client->getMemberLevel() & REGISTERED))
        throw ERR_NOTREGISTERED;
}

void Mode::modeCmd(Client *client)
{
    // Mode [<channel> [<o>]]

    // Check if the channel exists.
    if (_server->getChannelList().find(_args[1]) == _server->getChannelList().end())
        throw ERR_NOSUCHCHANNEL;
    // Check if the client is in the channel.
    std::string msgBuf = ":IRC 324 " + client->getNickname() + " " + _args[1] + " +nt";
    client->sendToClient(msgBuf);
}