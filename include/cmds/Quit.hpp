#ifndef COMMAND_QUIT_HPP
#define COMMAND_QUIT_HPP

#include "ACommand.hpp"

class Server;

class Quit : public ACommand
{
  private:
    Quit();
    Quit(Quit const &obj);
    Quit  &operator=(Quit const &obj);

  public:
    Quit(Server* serv);
    ~Quit();

    void exec(Client *client);

    void isValidFormat(void);
    void checkClientLevel(Client* client);
    void quitCmd(Client* client);
};


#endif