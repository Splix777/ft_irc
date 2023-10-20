#ifndef COMMAND_KICK_HPP
#define COMMAND_KICK_HPP

#include "ACommand.hpp"

#define MAX_KICK_NUM 1

class Kick : public ACommand
{
  private:
    Client* user;

    Kick(const Kick& obj);            // No use.
    Kick& operator=(const Kick& obj); // No use.
  public:
    Kick();
    Kick(Server* serv);
    ~Kick();
    void kickOn(Client* client);

    void isMemberInChannel(Client* client);

    int  isOperator(Client* client);
    void changeChannelStatus(Client* client);
    void sendMSG(Client* client);

    int isValidFormat(void);
    int checkClientLevel(Client* client);
    int determineFlag(void);
};

#endif // COMMAND_KICK_HPP
