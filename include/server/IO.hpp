#ifndef IO_HPP
# define IO_HPP

// Server Headers
# include "Channel.hpp"
# include "Client.hpp"
# include "Exception.hpp"
# include "Server.hpp"
# include "Parsing.hpp"
# include "Colors.hpp"

// csignal: signal
# include <csignal>
// Exception classes
# include <exception>
// iostream: cout, endl
# include <iostream>
// string: string
# include <string>

class IO
{
	private:
		// server
		bool	DEBUG;

		Server	*server;

		// Constructors not in use
		IO();
		IO(const IO &obj);
		IO &operator=(const IO &obj);

	public:
		// Constructor/Destructor
		IO(int argc, char *argv[]);
		~IO();

		// Socket Loop
		void run();
		// Terminates the server
		void stop();
		// Getter
		Server	*getServer() const;
};

#endif