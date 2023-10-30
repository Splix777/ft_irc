#include "IO.hpp"

IO::IO(int argc, char *argv[])
{
    // Initialize server
    this->server = new Server();
    if (argc == 3)
        this->server->initServer(argv[1], argv[2], false);
    else if (argc == 3 || (argc == 4 && strcmp(argv[3], "DEBUG") == 0))
        this->server->initServer(argv[1], argv[2], true);
    else if (argc == 4 && strcmp(argv[3], "DEBUG") != 0)
        this->server->initServer(argv[1], argv[2], argv[3], false);
    else if (argc == 5 && strcmp(argv[4], "DEBUG") == 0)
        this->server->initServer(argv[1], argv[2], argv[3], true);
    else
    {
        std::cerr << "ERROR: ./ircserv [host:port_network:password_network] <port> <password>" << std::endl;
        delete server;
        exit(1);
    }
}

IO::~IO()
{
    delete server;
}

void	IO::run()
{
    server->waitForEvents();
    for (std::vector<pollfd>::iterator it = server->getPollFdList().begin(); it != server->getPollFdList().end(); ++it)
    {
        if (it->revents == 0)
            continue ;
        if (it->revents & POLLHUP)
        {
            server->pollDisconnect(it->fd);
            break ;
        }
        if (it->revents & POLLIN)
        {
            if (it->fd == server->getSocketFd())
            {
                server->pollAccept();
                break ;
            }
            else
            {
                server->pollRead(it->fd);
                break ;
            }
        }
        if (it->revents & POLLOUT)
            server->pollSend(it->fd);
    }
}

void	IO::stop()
{
    server->terminate();
}

Server	*IO::getServer() const
{
    return (this->server);
}