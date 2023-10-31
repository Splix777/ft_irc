#ifndef COMMAND_KICK_HPP
#define COMMAND_KICK_HPP

#include "ACommand.hpp"

class Kick : public ACommand
{
	private:

		Kick();
		Kick(Kick const &obj);
		Kick &operator=(Kick const &obj);

	public:
		Kick(Server *serv);
		~Kick();

		void exec(Client *client);
		void cmdKick(Client *client);

		void isValidFormat(void);
};

#endif
