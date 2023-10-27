#ifndef COMMAND_MODE_HPP
#define COMMAND_MODE_HPP

#include "ACommand.hpp"

class Server;

class Mode : public ACommand
{
  private:
    Mode();
    Mode(Mode const &obj);
    Mode  &operator=(Mode const &obj);

  public:
    Mode(Server* serv);
    ~Mode();

    void exec(Client *client);

    void isValidFormat(void);
    void checkClientLevel(Client* client);
    void modeCmd(Client* client);
};


#endif