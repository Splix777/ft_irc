#include "Quit.hpp"

#include "Operation.hpp"

Quit::Quit()
{
}
Quit::Quit(Server* serv) : ACommand(serv)
{
}
Quit::~Quit()
{
}

void Quit::quitOn(Client* client)
{
    try
    {
        int clientFd = client->getSockFd();

        isValidFormat();
        msgToAllJoinedChannel(client);
        eraseFromList(client);
        // FD_CLR(clientFd, server->getReads());
        // FD_CLR(clientFd, server->getWrites());
        close(clientFd);
    }
    catch (int numeric)
    {
        std::stringstream sstm;
        sstm << numeric << " " << client->getNickname();
        std::string msgBuf = sstm.str();
        switch (numeric)
        {
        case ERR_UNKNOWNERROR:
            msgBuf += " QUIT :Invalid Format error !";
            break;
        default:
            break;
        }

        msgBuf += "\r\n";
        client->addSendBuff(msgBuf);
    }
}

void Quit::eraseFromList(Client* client)
{
    int clientFd = client->getSockFd();

    std::map<std::string, Channel*>::iterator it;
    for (it = client->getChannelList().begin(); it != client->getChannelList().end(); it++)
    {
        it->second->deleteClientElement(clientFd);
    }

    for (it = server->getChannelList().begin(); it != server->getChannelList().end(); it++)
    {
        if (it->second->getKickedList().find(clientFd) != it->second->getKickedList().end())
        {
            it->second->delKickedListElement(client);
        }
    }
    server->deleteClientElement(clientFd);
}

void Quit::msgToAllJoinedChannel(Client* client)
{
    // 	:<source> QUIT :Quit: <reason>
    std::string msgBuf = ":" + client->getNickname() + " QUIT :Quit";
    if (getSize() == 2)
    {
        msgBuf += cmd[2];
    }
    msgBuf += "\n";

    std::map<std::string, Channel*>::iterator it;
    for (it = client->getChannelList().begin(); it != client->getChannelList().end(); it++)
    {
        it->second->broadcast(msgBuf, client);
    }
}

int Quit::isValidFormat(void)
{
    // flag QUIT [reason]
    if (getSize() > 2)
    {
        throw ERR_UNKNOWNERROR;
    }
    return (TRUE);
}

int Quit::checkClientLevel(Client* client)
{
    int clientFd = client->getSockFd();
    (void)clientFd;
    return (TRUE);
}

int Quit::determineFlag(void)
{
    return (1);
}
