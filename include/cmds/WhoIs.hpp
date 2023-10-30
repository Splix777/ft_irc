#ifndef COMMAND_WHOIS_HPP
# define COMMAND_WHOIS_HPP

#include "ACommand.hpp"

class Server;

class WhoIs : public ACommand
{
  private:
    WhoIs();
    WhoIs(WhoIs const &obj);
    WhoIs  &operator=(WhoIs const &obj);

  public:
    WhoIs(Server* serv);
    ~WhoIs();

    void exec(Client *client);

    void isValidFormat(void);
    void checkClientLevel(Client* client);
    void whoIsCmd(Client* client);
};


#endif