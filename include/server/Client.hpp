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
# include "utils.hpp"

// Member Level using bit operation
// We use bit operation to check whether the client has passed the certification process.
// Using bit operation we can check each step of the certification process.
# define REGISTERED		(1 << 4)	// 00000000 00000000 00000000 00010000
# define USER_SET		(1 << 3)	// 00000000 00000000 00000000 00001000
# define NICK_SET		(1 << 2)	// 00000000 00000000 00000000 00000100
# define PASS_SET		(1 << 1)	// 00000000 00000000 00000000 00000010
# define UNREGISTERED	(1 << 0)	// 00000000 00000000 00000000 00000001

class Channel;

class Client
{
	private:
		// Socket Info
		int	socketFd;
		struct sockaddr_in	addr;

		// Debug
		bool DEBUG;

		// Client Info
		int	isMember;
		int	memberLevel;
		std::string	nickname;
		std::string	username;
		std::string	realname;

		// Buffers
		std::string	sendBuff;
		std::string	recvBuff;
		std::string	recvMsg;

		// Buffer Info
		static const int bufferSize = 512;
		char             buffer[bufferSize];

		std::map<std::string, Channel *>	channelList;

		// Constructor & Operator Overload not used
		Client();
		Client(Client const &copy);
		Client& operator=(Client const &copy);

	public:
		Client(const struct sockaddr_in addr, const int fd, bool DEBUG);
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
		std::map<std::string, Channel *>	&getChannelList();
		std::string							&getSendBuff();
		std::string							&getRecvBuff();
		std::string							&getRecvMsg();
		char								*getBuffer();
		int									getBufferSize() const;
		int									getFd() const;
		bool								getDebug() const;

		void addChannelElement(std::string const &channelName, Channel *newChannel);
		void deleteChannelElement(std::string key);

		void addSendBuff(std::string message);
		void addRecvBuff(std::string &message);
		void addRecvMsg(std::string &message);

		void sendToClient(std::string message);
		int  recvClient();

		void  leaveAllRooms();
};

#endif
