#ifndef COMMAND_PART_HPP
# define COMMAND_PART_HPP

# include "ACommand.hpp"

class Part : public ACommand
{
	private:
		std::vector<std::string> channelNames;

		Part();
		Part(Part const &src);
		Part &operator=(Part const &src);

	public:
		Part(Server *serv);
		~Part();

		void	exec(Client *client);

		// Validity Checks
		void	validCheck(Client *client);
		void 	isValidFormat();
		void    splitArgs();

		void	sendPart(Client *client);


};

#endif
