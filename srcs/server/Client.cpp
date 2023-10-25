#include "Client.hpp"
#include "Channel.hpp"
#include "IO.hpp"
#include <map>
#include <utility>

Client::~Client()
{
}

Client::Client(const struct sockaddr_in addr, const int fd, bool DEBUG) : socketFd(fd), addr(addr), isMember(0), memberLevel(UNREGISTERED)
{
    this->DEBUG = DEBUG;
    memset(buffer, 0, bufferSize);
}

void Client::setMemberLevel(int lev)
{
    if (lev == UNREGISTERED || lev == USER_SET || lev == NICK_SET || lev == PASS_SET || lev == REGISTERED)
        this->memberLevel |= lev;
}

void Client::setNickname(std::string const name)
{
    this->nickname = name;
}

void	Client::setUsername(std::string const name)
{
    this->username = name;
}

void	Client::setRealname(std::string const name)
{
    this->username = name;
}

void	Client::setSendBuff(std::string const &msg)
{
    this->sendBuff = msg;
}

void	Client::setRecvBuff(std::string const &msg)
{
    this->recvBuff = msg;
}

std::string	Client::getNickname() const
{
    return (this->nickname);
}

std::string	Client::getUsername() const
{
    return (this->username);
}

std::string	Client::getRealname() const
{
    return (this->username);
}

int	Client::getSockFd() const
{
    return (this->socketFd);
}

int	Client::getMemberLevel() const
{
    return (this->memberLevel);
}

int	Client::getisMember()
{
    if (this->getMemberLevel() == REGISTERED)
        return (1);
    else
        return (0);
}

std::map<std::string, Channel *>	&Client::getChannelList()
{
    return (this->channelList);
}

std::string	&Client::getSendBuff()
{
    return (this->sendBuff);
}

std::string	&Client::getRecvBuff()
{
    return (this->recvBuff);
}

std::string &Client::getRecvMsg()
{
    return (this->recvMsg);
}

char	*Client::getBuffer()
{
    return (this->buffer);
}

int Client::getBufferSize() const
{
    return (this->bufferSize);
}

int	Client::getFd() const
{
    return (this->socketFd);
}

bool	Client::getDebug() const
{
    return (this->DEBUG);
}

void	Client::addChannelElement(std::string const &channelName, Channel *newChannel)
{
    if (newChannel)
        this->channelList.insert(std::make_pair(channelName, newChannel));
}

void	Client::deleteChannelElement(std::string key)
{
    Channel *temp = this->channelList.find(key)->second;
    if (temp)
    {
        this->channelList.erase(key);
    }
}

void	Client::addSendBuff(std::string message)
{
    this->sendBuff += message;
}

void	Client::addRecvBuff(std::string &message)
{
    this->recvBuff += message;
}

void    Client::addRecvMsg(std::string &message)
{
    this->recvMsg += message;
}

void	Client::sendToClient(std::string message)
{
    addSendBuff(message);
    if (this->sendBuff.length() == 0)
        return;

    addSendBuff("\r\n");

    if (DEBUG)
        printDebug("[Server->Client]" + this->sendBuff);
}

int Client::recvClient()
{
    // Initialize buffer/Reset buffer
    memset(buffer, 0, bufferSize);
    int totalBytesReceived = 0;

    // Receive message from client
    totalBytesReceived = recv(socketFd, buffer, bufferSize, 0);

    // Check if there is an error
    if (totalBytesReceived <= 0)
        return (totalBytesReceived);

    // Convert buffer to string.
    std::string temp(buffer);

    // Print message from client for debugging purpose.
        if (DEBUG)
            printDebug("[Client->Server]" + temp);

    if (temp.find("\r\n") != std::string::npos)
    {
        // If the message is complete, then add it to the recvMsg.
        addRecvBuff(temp);
        addRecvMsg(this->getRecvBuff());
        this->getRecvBuff().clear();
        return (totalBytesReceived);
    }
    else
    {
        // If the message is incomplete, then add it to the recvBuff.
        addRecvBuff(temp);
        return (totalBytesReceived);
    }
}

void    Client::leaveAllRooms()
{
    for (std::map<std::string, Channel *>::iterator it = this->channelList.begin(); it != this->channelList.end(); it++)
    {
        it->second->deleteClientElement(this->getFd());
    }
    this->channelList.clear();
}