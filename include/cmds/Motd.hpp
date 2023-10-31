#ifndef COMMAND_MOTD_HPP
#define COMMAND_MOTD_HPP

#include "ACommand.hpp"

class Server;

class Motd : public ACommand
{
  private:

    Motd();
    Motd(Motd const &obj);
    Motd& operator=(Motd const &obj);

  public:
    Motd(Server *serv);
    ~Motd();

    void	exec(Client *client);
	void	cmdMotd(Client *client);

    void 	isValidFormat(void);
};

#endif