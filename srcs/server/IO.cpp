#include "IO.hpp"

IO::IO(char* s1, char* s2, bool DEBUG) : DEBUG(DEBUG)
{
    // Initialize server
    this->server = new Server();
    this->server->initServer(s1, s2, DEBUG);
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
                server->pollRead(it->fd);
        }
        if (it->revents & POLLOUT)
            server->pollSend(it->fd);
    }
}

void	IO::stop()
{
    server->terminate();
}