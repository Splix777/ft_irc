#include "WhoIs.hpp"
#include "IO.hpp"
#include "Replies.hpp"

WhoIs::WhoIs(Server *serv) : ACommand(serv)
{
}

WhoIs::~WhoIs()
{
}

void WhoIs::exec(Client *client)
{
    if (client->getDebug())
        printDebug("WhoIs Command Found, Executing WhoIs Command");

    try
    {
        isValidFormat();
        checkClientLevel(client);
        whoIsCmd(client);
        if (client->getDebug())
            printDebug("WhoIs Command Passed");
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " WhoIs :Invalid Format error !";
            break;

        case ERR_NEEDMOREPARAMS:
            msgBuf += " :Not enough parameters";
            break;

        case ERR_NOSUCHCHANNEL:
            msgBuf += " :No such channel";
            break;

        case ERR_NOSUCHNICK:
            msgBuf += " :No such nick/channel";
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

void WhoIs::isValidFormat(void)
{
    // WhoIs [<user> [<o>]]

    // Right now we only support WhoIs <user>
    // In the future WhoIs will support WhoIs <mask> and WhoIs <mask> o

    // Check if there are 2 arguments
    if (_args.size() > 3)
        throw ERR_UNKNOWNERROR;
    if (_args.size() < 2)
        throw ERR_NEEDMOREPARAMS;
}

void WhoIs::checkClientLevel(Client *client)
{
    if (!(client->getMemberLevel() & REGISTERED))
        throw ERR_NOTREGISTERED;
}

void WhoIs::whoIsCmd(Client *client)
{
    // WhoIs [<user> [<o>]]

    // Right now we only support WhoIs <user>
    // In the future WhoIs will support WhoIs <mask> and WhoIs <mask> o

    // Check if the user exists
    std::map<int, Client *> clients = _server->getClientList();

    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second->getNickname() == _args[1])
        {
            // Send the user info to the client
            client->sendToClient(_WHOIS(client->getNickname(), it->second->getNickname(), it->second->getUsername(), it->second->getHostName(), it->second->getRealname()));
            client->sendToClient(_EOFWHOIS(client->getNickname(), it->second->getNickname()));
            return;
        }
    }
    throw ERR_NOSUCHNICK;
}

