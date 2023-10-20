#include "Kick.hpp"

#include "Operation.hpp"

Kick::Kick()
{
}

Kick::Kick(Server* serv) : ACommand(serv)
{
}

Kick::~Kick()
{
}

void Kick::kickOn(Client* client)
{
    try
    {
        checkClientLevel(client);
        isValidFormat();
        isMemberInChannel(client);
        if (isOperator(client) == FALSE)
        {
            throw ERR_NOPRIVILEGES;
        }
        sendMSG(client);
        changeChannelStatus(this->user);
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getNickname();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " KICK :Invalid Format error !";
            break;

        case ERR_NOTREGISTERED:
            msgBuf += " :You have not registered. Register PASS, USER, NICK !";
            break;

        case ERR_NOSUCHCHANNEL:
            msgBuf += " " + cmd[2] + " :No such channel";
            break;

        case ERR_NOSUCHNICK:
            msgBuf += " " + cmd[3] + " :No such nick/channel";
            break;

        case ERR_NOPRIVILEGES:
            msgBuf += " :Permission Denied !";
            break;

        default:
            break;
        }

        msgBuf += "\r\n";
        client->addSendBuff(msgBuf);
    }
}

void Kick::isMemberInChannel(Client* client)
{
    if (client->getChannelList().find(cmd[2]) == client->getChannelList().end())
        throw ERR_NOSUCHCHANNEL;
    this->channel = client->getChannelList().find(cmd[2])->second;
    std::map<int, Client*>::iterator it;
    for (it = channel->getClientList().begin(); it != channel->getClientList().end(); it++)
    {
        if (it->second->getNickname() == cmd[3])
        {
            this->user = it->second;
            return;
        }
    }
    throw ERR_NOSUCHNICK;
}

int Kick::isOperator(Client* client)
{
    if (channel->getClientList().begin()->first == client->getSockFd())
    {
        return (TRUE);
    }
    return (FALSE);
}

void Kick::changeChannelStatus(Client* client)
{
    channel->addKickedListElement(client);
    channel->deleteClientElement(client->getSockFd());
    client->deleteChannelElement(channel->getChannelName());
}

void Kick::sendMSG(Client* client)
{
    // flag KICK <channel> <user> [reason]
    std::string msgBuf = ":" + client->getNickname() + " KICK " + this->channel->getChannelName() + " " + cmd[3];
    msgBuf += "\r\n";
    channel->broadcastWithMe(msgBuf, client);
}

int Kick::isValidFormat(void)
{
    if (getSize() != 4)
    {
        throw ERR_UNKNOWNERROR;
    }
    return (TRUE);
}

int Kick::checkClientLevel(Client* client)
{
    if ((client->getMemberLevel() & CERTIFICATION_FIN) == 0)
    {
        throw ERR_NOTREGISTERED;
    }
    return (TRUE);
}

int Kick::determineFlag(void)
{
    return (1);
}
