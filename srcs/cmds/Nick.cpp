#include "Nick.hpp"
#include "Replies.hpp"
#include "IO.hpp"

char Nick::invalid[8] = {' ', ',', '*', '?', '!', '@', '.', '#'};

Nick::Nick(Server* serv) : ACommand(serv)
{
}

Nick::~Nick()
{
}

void Nick::exec(Client *client)
{
    if (client->getDebug())
        printDebug("Nick Command Found, Executing Nick Command");
    try
    {
        isValidFormat();
        checkClientLevel(client);
        validCheck();
        checkUsedNick();
        setClientNick(client);
        if (((client->getMemberLevel() & REGISTERED) == REGISTERED) && flag == 0)
        {
            welcome(client);
            flag = 1;
        }
        if (client->getDebug())
            printDebug("Nick Command Passed");
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getSockFd();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " NICK :Invalid Format error !";
            break;

        case ERR_NOTREGISTERED:
            msgBuf += " :You have not registered.";
            break;

        case ERR_ERRONEUSNICKNAME:
            msgBuf += " " + _args[1] + " :Erroneus nickname ";
            break;

        case ERR_NICKNAMEINUSE:
            msgBuf += " " + _args[1] + " :Nickname is already in use";
            break;

        default:
            break;
        }
        client->sendToClient(msgBuf);
    }
    _command.clear();
    _args.clear();
}

void Nick::welcome(Client* client)
{
    // 001 <client> :<msg>
    client->sendToClient(":IRC 001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname());
}

void Nick::validCheck(void)
{
    // NICK <nickname>
    std::string nickName = _args[1];
    for (int i = 0; i < 8; i++)
    {
        if (nickName.find(invalid[i]) != std::string::npos)
            throw ERR_UNKNOWNERROR;
    }
    if (nickName[0] == '$' || nickName[0] == ':')
        throw ERR_UNKNOWNERROR;
    if (nickName.length() > 9)
        throw ERR_ERRONEUSNICKNAME;
}

void Nick::checkUsedNick(void)
{
    // NICK <nickname>
    std::map<int, Client *>::iterator cit;
    std::string	nickName = _args[1];

    for (cit = _server->getClientList().begin(); cit != _server->getClientList().end(); cit++)
    {
        if (cit->second->getNickname() == nickName)
            throw ERR_NICKNAMEINUSE;
    }
}

void Nick::isValidFormat(void)
{
    // NICK <nickname>
    if (_args.size() != 2 || _args[1].empty())
        throw ERR_UNKNOWNERROR;
}

void Nick::setClientNick(Client* client)
{
    // flag NICK <nickname>
    std::string nickName = _args[1];
    if ((client->getMemberLevel() & REGISTERED) == REGISTERED)
    {
        client->sendToClient(_NICK(client->getNickname(), nickName));
        std::map<std::string, Channel *>::iterator it;
        for (it = client->getChannelList().begin(); it != client->getChannelList().end(); it++)
        {
            it->second->broadcast(_NICK(client->getNickname(), nickName), client);
        }
    }
    client->setNickname(nickName);
    client->setMemberLevel(NICK_SET);

    if ((client->getMemberLevel() & (PASS_SET | USER_SET)) == (PASS_SET | USER_SET))
        client->setMemberLevel(REGISTERED);
}

void Nick::checkClientLevel(Client* client)
{
    if ((client->getMemberLevel() & PASS_SET) != PASS_SET)
        throw ERR_NOTREGISTERED;
}

