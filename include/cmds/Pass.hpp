#ifndef COMMAND_PASS_HPP
#define COMMAND_PASS_HPP

#include "ACommand.hpp"

#define ERR_PASSWDMISMATCH 464
#define ERR_ALREADYREGISTERED 462

class Server;

class Pass : public ACommand
{
  private:
    Pass();
    Pass(Pass const &obj);
    Pass  &operator=(Pass const &obj);

  public:
    Pass(Server* serv);
    ~Pass();

    void exec(Client *client);

    void passCmp(Client* client);

    void isValidFormat(void);
    void checkClientLevel(Client* client);
    void determineFlag(void);
};

#endif // COMMAND_PASS_HPP
