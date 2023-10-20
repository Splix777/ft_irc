#ifndef COMMAND_JOIN_HPP
#define COMMAND_JOIN_HPP

#include "ACommand.hpp"

#define ADD_LIMIT 1
#define MAX_MEMBER 10

class Join : public ACommand
{
  private:
    Join(const Join& obj);            // No use.
    Join& operator=(const Join& obj); // No use.
  public:
    Join();
    Join(Server* serv);
    ~Join();
    void joinOn(Client* client);

    int  isKicked(Client* client);
    int  isExistChannel(std::string channelName);
    void leaveAll(Client* client);
    void addClient(Client* client);
    void addChannel(std::string channelName);
    void sendMSG(Client* client);
    int  isAlreadyJoined(Client* client);

    int isValidFormat(void);
    int checkClientLevel(Client* client);
    int determineFlag(void);

    Channel* getChannel(std::string& channelName);
};

#endif // COMMAND_JOIN_HPP
