#include "IO.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool	g_status = TRUE;

static void sigintHandler(int signum) 
{
    if (signum == SIGINT)
    	g_status = FALSE;
}

int main(int argc, char *argv[])
{
	if (argc < 3 && argc > 5)
	{
		std::cerr << "ERROR: ./ircserv [host:port_network:password_network] <port> <password>" << std::endl;
		return (1);
	}

	signal(SIGINT, sigintHandler);

	IO IO(argc, argv);
	
	while (g_status && IO.getServer()->isRunning())
	{
		try
		{
			IO.run();
			usleep(1000);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	if (g_status == FALSE || IO.getServer()->isRunning() == FALSE)
    {
		std::cout << "Server is shutting down..." << std::endl;
        IO.stop();
	}

	return (0);
}
