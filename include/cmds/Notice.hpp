#ifndef COMMAND_NOTICE_HPP
# define COMMAND_NOTICE_HPP

# include "ACommand.hpp"

class Notice : public ACommand
{
	private:

		Notice();
		Notice(Notice const &src);
		Notice &operator=(Notice const &src);
	public:

		Notice(Server *serv);
		~Notice();

		//Type of Notice
		enum typeSend { USER, CHANNEL };

		void	exec(Client *client);

		// Validity Checks
		void			isValidFormat();
		void			checkClientLevel(Client *client);
		typeSend 		getTargetType();
		void			sendNotice(Client *client);
		bool			isUserinChannel(std::map<int, Client *>::iterator it_client, std::map<std::string, Channel *>::iterator it_channel);

};

#endif
