#include "Client.hpp"
#include "Channel.hpp"
#include "Operation.hpp"
#include <map>
#include <utility>

// Client::Client() : socketFd(-1), nickname(), username(), realname(), addr(), isMember(0)
// {
//     memset(buffer, 0, bufferSize);
//     setMemberLevel(UNREGISTERED);
// }

// Client::Client(int fd) : socketFd(fd), nickname(), username(), realname(), addr(), isMember(0)
// {
//     memset(&this->addr, 0, sizeof(this->addr));
//     memset(buffer, 0, bufferSize);
//     setMemberLevel(UNREGISTERED);
// }

Client::~Client()
{
}

// Client::Client(const struct sockaddr_in addr) : addr(addr), isMember(0)
// {
//     setMemberLevel(UNREGISTERED);
// }

Client::Client(const struct sockaddr_in addr, const int fd) : socketFd(fd), addr(addr), isMember(0)
{
    setMemberLevel(UNREGISTERED);
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

int	Client::getisMember() const
{
    return (this->isMember);
}

std::map<std::string, Channel*>	&Client::getChannelList()
{
    return (this->channelList);
}

std::string	&Client::getsendBuff()
{
    return (this->sendBuff);
}

std::string	&Client::getRecvBuff()
{
    return (this->recvBuff);
}

char	*Client::getBuffer()
{
    return (this->buffer);
}

int	Client::getFd() const
{
    return (this->socketFd);
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
        delete temp;
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

void	Client::sendToClient(std::string message)
{
    try
    {
        addSendBuff(message);
        if (this->sendBuff.length() == 0)
            return;
        std::cout << "[Server->Client]" << this->sendBuff << std::endl;
        int ret = send(socketFd, this->sendBuff.c_str(), this->sendBuff.length(), 0);
        if (ret < 0)
        {
            throw Exceptions::sendToClientException();
        }
        this->sendBuff.clear();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }
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
    std::cout << "[Client->Server]" << buffer << std::endl;

    // If there is no \r\n or \n in the message, add it to recvBuff and return.
    if (temp.find("\n") == std::string::npos)
    {
        addRecvBuff(temp);
        return (totalBytesReceived);
    }

    // If there is \r\n or \n in the message, split the message and add it to recvBuff.
    size_t pos = temp.find("\r\n");

    // If there is no \r\n, check if there is \n.
    if (pos == std::string::npos)
        pos = temp.find("\n");

    // Split the message and add it to recvBuff. We need to split the message because
    // we need to check if there is \r\n or \n in the message.
    // The \r\n or \n is used to check if the message is complete.
    while (pos != std::string::npos)
    {
        std::string message = temp.substr(0, pos);
        addRecvBuff(message);
        temp = temp.substr(pos + 1);
        pos = temp.find("\r\n");
        if (pos == std::string::npos)
            pos = temp.find("\n");
    }
    return (totalBytesReceived);
}
