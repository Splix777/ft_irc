#ifndef COMMAND_NICK_HPP
#define COMMAND_NICK_HPP

#include "ACommand.hpp"

#define TRUE 1
#define FALSE 0

class Server;

class Nick : public ACommand
{
  private:

    int	flag;
    static char	invalid[8];

    Nick();
    Nick(Nick const &obj);
    Nick& operator=(Nick const &obj);

  public:
    Nick(Server *serv);
    ~Nick();

    void	exec(Client *client);

    void 	validCheck(void);
    void 	checkUsedNick(void);
    void	setClientNick(Client* client);

    void	isValidFormat(void);
    void	checkClientLevel(Client* client);

    void	welcome(Client* client);
};

#endif