#ifndef COMMAND_PRVMSG_HPP
# define COMMAND_PRVMSG_HPP

# include "ACommand.hpp"
# include "Bot.hpp"
# include "Notice.hpp"

#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412

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
		void	sendPrivmsg(Client *client);
};

#endif
