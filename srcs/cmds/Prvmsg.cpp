#include "Prvmsg.hpp"
#include "IO.hpp"

Prvmsg::Prvmsg(Server *serv) : Notice(serv)
{
}

Prvmsg::~Prvmsg()
{
}

void Prvmsg::exec(Client *client)
{
	if (client->getDebug())
		printDebug("PRIVMSG Command Found, Executing PRIVMSG Command");
	try
	{
		validCheck(client);
		sendNotice(client);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Prvmsg::sendPrivmsg(Client *client)
{
	(void)client;
}

void Prvmsg::validCheck(Client *client)
{
	try
	{
		isValidFormat();
		checkClientLevel(client);
		// checkKicked();
		// checkBanned();
	}
	catch (int numeric)
	{
		// Dont send error in NOTICE command

		std::stringstream sstm;
		sstm << numeric << " " << client->getSockFd();
		std::string msgBuf = sstm.str();
		switch (numeric)
		{
		case ERR_NEEDMOREPARAMS:
			throw ERR_NEEDMOREPARAMS;
		    msgBuf += " NOTICE :Not enough parameters";
		    break;

		case ERR_NOTREGISTERED:
			throw ERR_NOTREGISTERED;
		    msgBuf += " :You have not registered";
		    break;

		default:
			throw 0;
		    break;
		}
		client->sendToClient(msgBuf);
		_command.clear();
		_args.clear();
		throw ERR_NOTREGISTERED;
	}
}

// 3.3.1 Private messages

//       Command: PRIVMSG
//    Parameters: <msgtarget> <text to be sent>

//    PRIVMSG is used to send private messages between users, as well as to
//    send messages to channels.  <msgtarget> is usually the nickname of
//    the recipient of the message, or a channel name.

//    The <msgtarget> parameter may also be a host mask (#<mask>) or server
//    mask ($<mask>).  In both cases the server will only send the PRIVMSG
//    to those who have a server or host matching the mask.  The mask MUST
//    have at least 1 (one) "." in it and no wildcards following the last
//    ".".  This requirement exists to prevent people sending messages to
//    "#*" or "$*", which would broadcast to all users.  Wildcards are the
//    '*' and '?'  characters.  This extension to the PRIVMSG command is
//    only available to operators.

//    Numeric Replies:

//            ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
//            ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
//            ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
//            ERR_NOSUCHNICK
//            RPL_AWAY



// Kalt                         Informational                     [Page 23]

// RFC 2812          Internet Relay Chat: Client Protocol        April 2000


//    Examples:

//    :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?
//                                    ; Message from Angel to Wiz.

//    PRIVMSG Angel :yes I'm receiving it !
//                                    ; Command to send a message to Angel.

//    PRIVMSG jto@tolsun.oulu.fi :Hello !
//                                    ; Command to send a message to a user
//                                    on server tolsun.oulu.fi with
//                                    username of "jto".

//    PRIVMSG kalt%millennium.stealth.net@irc.stealth.net :Are you a frog?
//                                    ; Message to a user on server
//                                    irc.stealth.net with username of
//                                    "kalt", and connected from the host
//                                    millennium.stealth.net.

//    PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
//                                    ; Message to a user on the local
//                                    server with username of "kalt", and
//                                    connected from the host
//                                    millennium.stealth.net.

//    PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
//                                    ; Message to the user with nickname
//                                    Wiz who is connected from the host
//                                    tolsun.oulu.fi and has the username
//                                    "jto".

//    PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
//                                    ; Message to everyone on a server
//                                    which has a name matching *.fi.

//    PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
//                                    ; Message to all users who come from
//                                    a host which has a name matching
//                                    *.edu.a