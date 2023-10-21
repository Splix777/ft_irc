#include "Nick.hpp"
#include "Operation.hpp"

char Nick::invalid[8] = {' ', ',', '*', '?', '!', '@', '.', '#'};

Nick::Nick(Server* serv) : ACommand(serv)
{
}

Nick::~Nick()
{
}

void Nick::exec(Client *client)
{
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
            msgBuf += " :You have not registered. Register PASS, USER, NICK !";
            break;

        case ERR_ERRONEUSNICKNAME:
            msgBuf += " " + _args[2] + " :Erroneus nickname ";
            break;

        case ERR_NICKNAMEINUSE:
            msgBuf += " " + _args[2] + " :Nickname is already in use";
            break;

        default:
            break;
        }
        msgBuf += "\r\n";
        client->sendToClient(msgBuf);
    }
}

void Nick::welcome(Client* client)
{
    // 001 <client> :<msg>
    std::string userName = _args[2];
    std::string serverName = static_cast<std::string>(SERVERNAME);
    std::string msgBuf = "001 " + userName + " :Welcome, " + userName + "! Your host is " + serverName + "\r\n";
    client->sendToClient(msgBuf);
}

void Nick::validCheck(void)
{
    // flag NICK <nickname>
    std::string nickName = _args[2];
    for (int i = 0; i < 8; i++)
    {
        if (nickName.find(invalid[i]) != std::string::npos)
            throw ERR_UNKNOWNERROR;
    }
    if (nickName[0] == '$' || nickName[0] == ':')
        throw ERR_UNKNOWNERROR;
}

void Nick::checkUsedNick(void)
{
    // flag NICK <nickname>
    std::map<int, Client *>::iterator cit;
    std::string	nickName = _args[2];

    for (cit = _server->getClientList().begin(); cit != _server->getClientList().end(); cit++)
    {
        if (cit->second->getNickname() == nickName)
            throw ERR_NICKNAMEINUSE;
    }
}

void Nick::isValidFormat(void)
{
    // flag NICK <nickname>
    if (_args.size() != 3 || _args[2].empty())
        throw ERR_UNKNOWNERROR;
}

void Nick::setClientNick(Client* client)
{
    // flag NICK <nickname>
    std::string nickName = _args[2];
    if ((client->getMemberLevel() & REGISTERED) == REGISTERED)
    {
        std::string msg = ":" + client->getNickname() + " NICK :" + nickName + "\r\n";
        client->sendToClient(msg);
        std::map<std::string, Channel *>::iterator it;
        for (it = client->getChannelList().begin(); it != client->getChannelList().end(); it++)
        {
            it->second->broadcast(msg, client);
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

int Nick::determineFlag(void)
{
	if (_args[0] == "1")
		return (1);
	return (0);
}
