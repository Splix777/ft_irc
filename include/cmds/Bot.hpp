#ifndef BOT_HPP
# define BOT_HPP

#include "ACommand.hpp"

class Server;

class Bot : public ACommand
{
	private:

		Bot();
		Bot(Bot const &src);
		Bot &operator=(Bot const &src);
		
	public:

		Bot(Server *serv);
		~Bot();

		void	exec(Client *client);
		void	cmdBot(Client *client);

        void    isValidFormat();
};

#endif