#ifndef COMMAND_PART_HPP
#define COMMAND_PART_HPP

#include "ACommand.hpp"

class Part : public ACommand
{
  private:
    Part(const Part& obj);            // No use.
    Part& operator=(const Part& obj); // No use.

  public:
    Part();
    Part(Server* serv);
    ~Part();
    void partOn(Client* client);

    int  isLastMember(void);
    void eraseChannel(Client* client);
    void channelClose(void);
    void operatorChanged(Client* client);

    int isValidFormat(void);
    int checkClientLevel(Client* client);
    int determineFlag(void);
};

#endif // COMMAND_PART_HPP
