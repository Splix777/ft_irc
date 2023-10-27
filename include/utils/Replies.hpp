#ifndef REPLIES_HPP
#define REPLIES_HPP

//User
# define _user(nickname, realname, host) (":" + nickname + "!" + realname + "@" + host)
# define _NOTREGISTERED(host, client) (":" + host + " 462 " + client + " :You may not reregister.")

// PART
# define _PART(user, channel, reason) (user + " PART " + channel + " " + (reason.empty() ? "." : reason ))

// GENERAL
# define _NEEDMOREPARAMS(host, client, command) (":" + host + " 461 " + client + " " + command + " :Not enough parameters.")

// CHANNELS
# define _NOTONCHANNEL(host, client, channel) (":" + host + " 442 " + client + " #" + channel + " :The user is not on this channel.")
# define _CHANNELNOTEXIST(host, client, channel) (":" + host + " 403 " + client + " #" + channel + " :No such channel")

// NOTICE
# define _NOTICE(nickname, realname, host, target, message) (":" + nickname + "!" + realname + "@" + host + " NOTICE " + target + message)

// PRIVMSG

#endif