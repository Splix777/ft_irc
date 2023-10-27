#ifndef COMMAND_JOIN_HPP
# define COMMAND_JOIN_HPP

# include "ACommand.hpp"
# include "utils.hpp"

# define MAX_MEMBER 10

class Server;

class Join : public ACommand
{
    private:

        std::vector<std::string> channelNames;
        std::vector<std::string> channelPassword;

		Join();
		Join(Join const &src);
		Join &operator=(Join const &src);
	public:

		Join(Server *serv);
		~Join();

		void	exec(Client *client);

		// Validity Checks
		void	validCheck(Client *client);
		void	isValidFormat();
		void	checkClientLevel(Client *client);
		void	splitArgs();

		// Attempt to Join Channels
		void	joinChannels(Client *client);
		void	parseName(std::string const &name);
		void	parsePassword(std::string const &password);
		bool	channelExists(std::string const &name);
		void	createChannel(std::string const &name, std::string const &password, Client *client);
		void	addClientToChannel(std::string const &name, std::string const &password, Client *client);

		// Welcome Message
		void	welcome(Client *client, std::string const &channelName);
};

#endif
