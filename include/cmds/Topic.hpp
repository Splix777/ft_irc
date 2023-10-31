#ifndef COMMAND_TOPIC_HPP
# define COMMAND_TOPIC_HPP

# include "ACommand.hpp"

class Server;

class Topic : public ACommand
{
    private:

		Topic();
		Topic(Topic const &src);
		Topic &operator=(Topic const &src);
		
	public:

		Topic(Server *serv);
		~Topic();

		void	exec(Client *client);

        void    isValidFormat();
		void    cmdTopic(Client *client);
};

#endif
