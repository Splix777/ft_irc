#ifndef COMMAND_QUIT_HPP
#define COMMAND_QUIT_HPP

#include "ACommand.hpp"

class Quit : public ACommand
{
  private:
    Quit(const Quit& obj);            // No use.
    Quit& operator=(const Quit& obj); // No use.
  public:
    Quit();
    Quit(Server* serv);
    ~Quit();
    void quitOn(Client* client);
    void eraseFromList(Client* client);
    void msgToAllJoinedChannel(Client* client);

    int isValidFormat(void);
    int checkClientLevel(Client* client);
    int determineFlag(void);
};

#endif // COMMAND_QUIT_HPP
