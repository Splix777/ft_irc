#ifndef COMMAND_PING_HPP
#define COMMAND_PING_HPP

#include "ACommand.hpp"

class Ping : public ACommand
{
  private:
    Ping(const Ping& obj);            // No use.
    Ping& operator=(const Ping& obj); // No use.

  public:
    Ping();
    Ping(Server* serv);
    ~Ping();
    void pingOn(Client* client);
    void pong(Client* client);

    int isValidFormat(void);
    int checkClientLevel(Client* client);
    int determineFlag(void);
};

#endif // COMMAND_PING_HPP
