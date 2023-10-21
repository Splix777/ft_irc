#ifndef OPERATION_HPP
#define OPERATION_HPP

// Server Headers
# include "Channel.hpp"
# include "Client.hpp"
# include "Exception.hpp"
# include "Server.hpp"
# include "Parsing.hpp"
# include "Colors.hpp"
// Command Headers
# include "ACommand.hpp"
# include "Bot.hpp"
# include "Join.hpp"
# include "Kick.hpp"
# include "Nick.hpp"
# include "Notice.hpp"
# include "Part.hpp"
# include "Pass.hpp"
# include "Ping.hpp"
# include "Prvmsg.hpp"
# include "Quit.hpp"
# include "User.hpp"

// csignal: signal
# include <csignal>
// Exception classes
# include <exception>
// iostream: cout, endl
# include <iostream>
// string: string
# include <string>
// sys/ioctl.h: ioctl
# include <sys/ioctl.h>
// vector: vector
# include <vector>
// map: map
# include <map>

# define ERROR -1
# define FAIL 0
# define SUCCESS 1

// Member Level using bit operation
// We use bit operation to check whether the client has passed the certification process.
// Using bit operation we can check each step of the certification process.
# define REGISTERED		(1 << 4)	// 00000000 00000000 00000000 00010000
# define USER_SET		(1 << 3)	// 00000000 00000000 00000000 00001000
# define NICK_SET		(1 << 2)	// 00000000 00000000 00000000 00000100
# define PASS_SET		(1 << 1)	// 00000000 00000000 00000000 00000010
# define UNREGISTERED	(1 << 0)	// 00000000 00000000 00000000 00000001

class Operation
{
	private:
		// server
		bool	DEBUG;

		int	setFd;
		int	result;
		Server	*server;
		Parsing parser;

		// commands
		User	*cmdUser;
		// Quit	*cmdQuit;
		// Prvmsg	*cmdPrvmsg;
		// Ping	*cmdPing;
		Pass	*cmdPass;
		// Part	*cmdPart;
		// Notice	*cmdNotice;
		// Nick	*cmdNick;
		// Kick	*cmdKick;
		// Join	*cmdJoin;

		std::map<std::string, ACommand *>	cmdMap;

		// Constructors not in use
		Operation();
		Operation(const Operation &obj);
		Operation &operator=(const Operation &obj);

	public:
		// Constructor/Destructor
		Operation(char *s1, char *s2, bool DEBUG);
		~Operation();

		// Add Commands to map
		void initCommandMap();
		// Socket Loop
		void pollLoop();
		// Terminates the server
		void terminate();
};

#endif