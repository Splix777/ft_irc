#ifndef COMMAND_PING_HPP
#define COMMAND_PING_HPP

#include "ACommand.hpp"

class Server;

class Ping : public ACommand
{
	private:
		Ping();
		Ping(Ping const &src);
		Ping &operator=(Ping const &src);

	public:
		Ping(Server *serv);
		~Ping();

		void 	exec(Client *client);
		void    isValidFormat();
		void	sendPong(Client *client);
};

#endif
