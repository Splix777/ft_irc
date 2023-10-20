#ifndef SERVER_HPP
# define SERVER_HPP

// arpa/inet.h : inet_ntoa (IP address)
# include <arpa/inet.h>
// exception: exception
# include <exception>
// fcntl.h: fcntl (file control)
# include <fcntl.h>
// iostream: cout, endl
# include <iostream>
// cstdlib: atoi (string to integer)
# include <cstdlib>
// netinet/in.h: sockaddr_in (socket address)
# include <netinet/in.h>
// string: string
# include <string>
// poll.h: poll (polling)
# include <poll.h>
// sys/socket.h & sys/types.h: socket, bind, listen, accept
# include <sys/socket.h>
# include <sys/types.h>
// unistd.h: close (close file descriptor)
# include <unistd.h>
// map: map
# include <map>
// vector: vector
# include <vector>
// utility: pair (pair.first, pair.second)
# include <utility>


// Server Headers
# include "ACommand.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Exception.hpp"
# include "Colors.hpp"
# include "Parsing.hpp"
# include "utils.hpp"

# define MAX_FD 1000
# define BUFFER_SIZE 1024
# define SERVERNAME "IRC Server"

class Server
{
  private:
	// Server Info
    int         port;
    std::string password;

	// MAX_FD (Users + Server)
    int maxFd;

	// Socket Info
    int                socketFd;
    struct sockaddr_in addr;

    // Parsing
    Parsing parser;

	// pollFdList
    std::vector<pollfd> pollFdList;

	// Client and Channel List
    std::map<int, Client *>          clientList;
    std::map<std::string, Channel *> channelList;

    // Debug Mode
    bool DEBUG;

	// Will be used in the future.
    Server(Server const &copy);
    Server& operator=(Server const &copy);

  public:
    Server();
    ~Server();

    // Server Init
    void initServer(char* port, char* password, bool DEBUG);
    void setServAddr();

    // Socket Init
    void setSocket();
    void bindSocket();
    void setPollFds();

    // pollLoop
    void waitForEvents();
    void pollAccept();
    void pollDisconnect(int fd);
    void pollRead(int fd, std::map<std::string, ACommand *>	cmdMap);
    void pollSend(int fd);

    // utils
    void addChannelElement(std::string const channelName, Channel* newChannel);
    void deleteChannelElement(std::string const channelName);
    void deleteClientElement(const int fd);
    void parseArgs(char* port, char* password);


    // getter
    int	getPort() const;
    int	getSocketFd() const;
    int	getCurrentMaxFd() const;
    std::string getPassword() const;

    struct sockaddr_in	getAddr() const;
    
    std::vector<pollfd>					&getPollFdList();
    std::map<int, Client *>				&getClientList();
    std::map<std::string, Channel *>	&getChannelList();

    void  leaveAll(int fd);
    void  terminate();
};

#endif