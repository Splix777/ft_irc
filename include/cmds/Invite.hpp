#ifndef COMMAND_INVITE_HPP
#define COMMAND_INVITE_HPP

#include "ACommand.hpp"

class Invite : public ACommand
{
	private:

		Invite();
		Invite(Invite const &obj);
		Invite &operator=(Invite const &obj);

	public:
		Invite(Server *serv);
		~Invite();

		void exec(Client *client);
		void cmdInvite(Client *client);

		void isValidFormat(void);
};

#endif
