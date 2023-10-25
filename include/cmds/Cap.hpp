#ifndef COMMAND_CAP_HPP
# define COMMAND_CAP_HPP

# include "ACommand.hpp"
# include "utils.hpp"

class Server;

class Cap : public ACommand
{
    private:


		Cap();
		Cap(Cap const &src);
		Cap &operator=(Cap const &src);
	public:

		Cap(Server *serv);
		~Cap();

		void	exec(Client *client);

        void    isValidFormat();
        void    checkClientLevel(Client *client);
        void    serverCapabilities(Client *client);
};

#endif
