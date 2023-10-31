#ifndef REPLIES_HPP
#define REPLIES_HPP

// INIT
# define _WELCOME(userName, serverName) (":IRC 001 " + userName + " :Welcome, " + userName + "! Your host is " + serverName)
# define _RHOST(nick, servername, version) (":IRC 002 " + nick + " :Your host is " + servername + ", running version " + version)
# define _CREATED(nick, datetime) (":IRC 003 " + nick + " :This server was created on " + datetime)
# define _INFO(nick, servername, version, user_modes, chan_modes, chan_param_modes) (":IRC 004 " + nick + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes)
# define _ISUPPORT(nick, tokens) (":IRC 005 " + nick + " " + tokens " :are supported by this server")

// USER
# define _user(nickname, realname, host) (":" + nickname + "!" + realname + "@" + host)
# define _NOTREGISTERED(host, client) (":" + host + " 462 " + client + " :You may not reregister.")

// PART
# define _PART(user, channel, reason) (user + " PART " + channel + " " + (reason[0] ? reason : "."))

// GENERAL
# define _NEEDMOREPARAMS(host, client, command) (":" + host + " 461 " + client + " " + command + " :Not enough parameters.")

// CHANNELS
# define _NOTONCHANNEL(host, client, channel) (":" + host + " 442 " + client + " #" + channel + " :The user is not on this channel.")
# define _CHANNELNOTEXIST(host, client, channel) (":" + host + " 403 " + client + " " + channel + " :No such channel")

// NOTICE
# define _NOTICE(nickname, realname, host, target, message) (":" + nickname + "!" + realname + "@" + host + " NOTICE " + target + message)

// PRIVMSG
# define _PRIVMSG(nick, realname, host, target, message) (":" + nick + "!" + realname + "@" + host + " PRIVMSG " + target + message)
# define _NONICKORCHANNEL(nick, target) ("401 " + nick + " " + target + " :No such nick/channel.")
# define _NOTEXTTOSEND(nick) ("412 " + nick + " :Only operators and voiced can talk")

// LIST
# define _INITLIST(nick) (":IRC 321 " + nick + " : Channel :Users Name")
# define _EOFLIST(nick) (":IRC 323 " + nick + " :End of /LIST")
# define _PRIVATETOPIC(nick, channelName, numberClients) (":IRC 322 " + nick + " " + channelName + " " + numberClients + " :" + ":The topic of this channel is private.")
# define _LIST(nick, channelName, numberClients, topic) (":IRC 322 " + nick + " " + channelName + " " + numberClients + " :" + (topic.empty() ? ":No topic set for this channel yet." : topic))

// QUIT
# define _QUIT(nick, message) ("ERROR :Closing Link: " + nick + " (" + message + ")")

// JOIN
# define _JOIN(nick, target) (":" + nick + " JOIN :" + target)

// NAMES
# define _NAMES(client, channel, list_of_nicks) (":IRC 353 " + client + " = " + channel + " :" + list_of_nicks)
# define _EOFNAMES(client, channel) (":IRC 366 " + client + " " + channel + " :End of /NAMES list.")

// NICK
//# define _NICK(oldnick, username, host, newNickname) (":" + oldnick + "!" + username + "@" + host + " NICK " +  newNickname + "\r\n")
# define _NICK(nick, newNickname) (":" + nick + " NICK :" + newNickname)

// MODES
#define _MODESET(channel, mode) (":IRC MODE " + channel + " " + mode)
#define _USERMODESET(nick, mode) (":" + nick + " MODE " + nick + " :" + mode)
#define _MODESETWITHPARAM(channel, mode, param) (":IRC MODE " + channel + " " + mode + " " + param)
#define _INVALIDMODEPARAM(nick, channel, mode, param) ("696 " + nick + " " + channel + " " + mode + " " + param + " : Invalid parameter")

// PING
#define _PONG(nick, realname, host, data) (":" + nick + "!" + realname + "@" + host + " PONG :" + data)
// WHO
# define _WHO(client, channel, username, host, server, nick, flags, hopcount, realname) (":IRC 352 " + client + " " + channel + " " + username + " " + host + " " + server + " " + nick + " " + flags + " :" + hopcount + " " + realname)
# define _EOFWHO(client, channel) (":IRC 315 " + client + " " + channel + " :End of /WHO list.")

// WHOIS
# define _WHOIS(client, nick, username, host, realname) (":IRC 311 " + client + " " + nick + " " + username + " " + host + " * :" + realname)
# define _EOFWHOIS(client, nick) (":IRC 318 " + client + " " + nick + " :End of /WHOIS list.")

// MOTD
#define _MOTDSTART(nick, servername) (":IRC 375 " + nick + " :- " + servername)
#define _ENDMOTD(nick) (":IRC 376 " + nick + " :End of /MOTD command.")
#define _MOTD(nick, text) (":IRC 372 " + nick + " :" + text)

#endif