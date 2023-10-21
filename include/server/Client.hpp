#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <map>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <string>
# include <sys/socket.h>
# include <cstring>

# include "Exception.hpp"

class Channel;

class Client
{
	private:
		// Socket Info
		int	socketFd;
		struct sockaddr_in	addr;

		// Client Info
		int	isMember;
		int	memberLevel;
		std::string	nickname;
		std::string	username;
		std::string	realname;

		// Buffers
		std::string	sendBuff;
		std::string	recvBuff;

		// Buffer Info
		static const int bufferSize = 1024;
		char             buffer[bufferSize];

		std::map<std::string, Channel *>	channelList;

		// Constructor & Operator Overload not used
		Client(const Client& ref);
		Client& operator=(const Client& ref);

	public:

		// Client();
		// Client(int fd);
		// Client(const struct sockaddr_in addr);
		Client(const struct sockaddr_in addr, const int fd);
		~Client();

		// setter
		void setMemberLevel(int lev);
		void setNickname(std::string const name);
		void setUsername(std::string const name);
		void setRealname(std::string const name);
		void setSendBuff(std::string const &msg);
		void setRecvBuff(std::string const &msg);

		// getter
		std::string                         getNickname() const;
		std::string                         getUsername() const;
		std::string                         getRealname() const;
		int                                 getSockFd() const;
		int                                 getMemberLevel() const;
		int                                 getisMember();
		std::map<std::string, Channel*>		&getChannelList();
		std::string							&getsendBuff();
		std::string							&getRecvBuff();
		char								*getBuffer();
		int									getFd() const;

		void addChannelElement(std::string const &channelName, Channel *newChannel);
		void deleteChannelElement(std::string key);

		void addSendBuff(std::string message);
		void addRecvBuff(std::string &message);

		void sendToClient(std::string message);
		int  recvClient();

};

#endif // CAN_CLIENT_HPP
