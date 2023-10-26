#ifndef COMMAND_WHO_HPP
#define COMMAND_WHO_HPP

#include "ACommand.hpp"

class Server;

class Who : public ACommand
{
  private:
    Who();
    Who(Who const &obj);
    Who  &operator=(Who const &obj);

  public:
    Who(Server* serv);
    ~Who();

    void exec(Client *client);

    void isValidFormat(void);
    void checkClientLevel(Client* client);
    void whoCmd(Client* client);
};


#endif