#include "Bot.hpp"
#include "Replies.hpp"
#include "IO.hpp"

Bot::Bot(Server *serv) : ACommand(serv)
{
}

Bot::~Bot()
{
}

void Bot::exec(Client *client)
{
	if (client->getDebug())
		printDebug("Bot Command Found, Executing Bot Command");

	try
	{
		isValidFormat();
		cmdBot(client);
		if (client->getDebug())
			printDebug("Bot Command Passed");
	}
	catch (int numeric)
	{
		std::stringstream sstm;
		sstm << numeric << " " << client->getSockFd();
		std::string msgBuf = sstm.str();
		switch (numeric)
		{
		case ERR_UNKNOWNERROR:
			msgBuf += ":Invalid Format error!, /BOT HELP";
			break;

		default:
			break;
		}
	}
	_command.clear();
	_args.clear();
}

void Bot::isValidFormat()
{
	if (_args.size() != 2)
		throw ERR_UNKNOWNERROR;
}

void Bot::cmdBot(Client *client)
{
	std::string cmd = _args[1];
	// Ignore case
	for (size_t i = 0; i < cmd.length(); i++)
	{
		cmd[i] = std::tolower(cmd[i]);
	}
	if (cmd == "help")
	{
		client->sendToClient(_PRIVMSG(std::string("BOT"), std::string("BOT"), "IRC", client->getNickname(), ": Aviable commands, Dice, Time, Joke"));
	}
	else if (cmd == "dice")
	{
		int random = (std::rand() % 6) + 1;
		std::stringstream rmsg;
		rmsg << "DICE: " << random;
		client->sendToClient(_PRIVMSG(std::string("BOT"), std::string("BOT"), "IRC", client->getNickname(), rmsg.str()));
	}
	else if (cmd == "time")
	{
		time_t now;
		struct tm timeinfo;
		time(&now);
		localtime_r(&now, &timeinfo);
		// Temp buffer to store time
		char buffer[80];

		// Format with strftime
		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &timeinfo);
		std::string str(buffer);
		client->sendToClient(_PRIVMSG(std::string("BOT"), std::string("BOT"), "IRC", client->getNickname(), std::string(buffer)));
	}
	else if (cmd == "joke")
	{
		int joke_number = (std::rand() % 10);
		std::string jokes[] = {
			": Why don't skeletons fight each other? They don't have the guts.",
			": Parallel lines have so much in common. It’s a shame they’ll never meet.",
			": What did one wall say to the other wall? I'll meet you at the corner!",
			": I told my wife she was drawing her eyebrows too high. She looked surprised.",
			": I'm on a whiskey diet. I've lost three days already.",
			": I used to play piano by ear, but now I use my hands.",
			": Why do bees have sticky hair? Because they always use honeycombs.",
			": I don't trust stairs because they're always up to something.",
			": Why don't scientists trust atoms? Because they make up everything!",
			": Did you hear about the mathematician who's afraid of negative numbers? He'll stop at nothing!"};

		client->sendToClient(_PRIVMSG(std::string("BOT"), std::string("BOT"), "IRC", client->getNickname(), jokes[joke_number]));
	}
}