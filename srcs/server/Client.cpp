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

void	Client::sendToClient()
{
    // try
    // {
    //     if (this->sendBuff.length() == 0)
    //         return;
    //     std::cout << "[Server->Client]" << this->sendBuff << std::endl;
    //     int ret = send(socketFd, this->sendBuff.c_str(), this->sendBuff.length(), 0);
    //     if (ret < 0)
    //     {
    //         throw Exceptions::sendToClientException();
    //     }
    //     this->sendBuff.clear();
    // }
    // catch (const std::exception& e)
    // {
    //     std::cerr << e.what() << "\n";
    // }
}

int Client::recvClient()
{
    int totalBytesReceived = 0;
    std::string accumulatedData; // Accumulate data received so far

    while (1)
    {
        int ret = recv(this->socketFd, this->buffer, bufferSize, 0);
        if (ret <= 0)
        {
            return totalBytesReceived; // Return total bytes received so far
        }

        accumulatedData += std::string(this->buffer, ret); // Append received data

        size_t newlinePos = accumulatedData.find("\r\n");
        if (newlinePos != std::string::npos)
        {
            // Found a complete message, process it and return
            std::string recvMsg = accumulatedData.substr(0, newlinePos + 2); // Include the "\r\n"
            addRecvBuff(recvMsg);
            return totalBytesReceived + ret; // Return total bytes received so far
        }

        totalBytesReceived += ret;
    }
}


