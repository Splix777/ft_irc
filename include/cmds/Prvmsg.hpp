#ifndef COMMAND_PRVMSG_HPP
# define COMMAND_PRVMSG_HPP

# include "ACommand.hpp"
# include "Bot.hpp"
# include "Notice.hpp"

class Prvmsg : public Notice
{
	private:

		Prvmsg();
		Prvmsg(Prvmsg const &src);
		Prvmsg &operator=(Prvmsg const &src);
	public:

		Prvmsg(Server *serv);
		~Prvmsg();

        void	exec(Client *client);
        void validCheck(Client *client);
		void sendPrivmsg(Client *client);
};

#endif
