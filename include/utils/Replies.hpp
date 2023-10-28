#ifndef REPLIES_HPP
#define REPLIES_HPP

//User
# define _user(nickname, realname, host) (":" + nickname + "!" + realname + "@" + host)
# define _NOTREGISTERED(host, client) (":" + host + " 462 " + client + " :You may not reregister.")

// PART
# define _PART(user, channel, reason) (user + " PART " + channel + " " + (reason[0] ? reason : "."))

// GENERAL
# define _NEEDMOREPARAMS(host, client, command) (":" + host + " 461 " + client + " " + command + " :Not enough parameters.")

// CHANNELS
# define _NOTONCHANNEL(host, client, channel) (":" + host + " 442 " + client + " #" + channel + " :The user is not on this channel.")
# define _CHANNELNOTEXIST(host, client, channel) (":" + host + " 403 " + client + " #" + channel + " :No such channel")

// NOTICE
# define _NOTICE(nickname, realname, host, target, message) (":" + nickname + "!" + realname + "@" + host + " NOTICE " + target + message)

// PRIVMSG
# define _PRIVMSG(nick, realname, host, target, message) (":" + nick + "!" + realname + "@" + host + " PRIVMSG " + target + message)
# define _NONICKORCHANNEL(nick, target) ("401 " + nick + " " + target + " :No such nick/channel.")

// LIST
# define _INITLIST(nick) (":IRC 321 " + nick + " : Channel :Users Name")
# define _EOFLIST(nick) (":IRC 323 " + nick + " :End of /LIST")
# define _PRIVATETOPIC(nick, channelName, numberClients) (":IRC 322 " + nick + " " + channelName + " " + numberClients + " :" + ":The topic of this channel is private.")
# define _LIST(nick, channelName, numberClients, topic) (":IRC 322 " + nick + " " + channelName + " " + numberClients + " :" + (topic.empty() ? ":No topic set for this channel yet." : topic))

#endif