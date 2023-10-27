#ifndef REPLIES_HPP
#define REPLIES_HPP

//User
# define _user(nickname, realname, host) (":" + nickname + "!" + realname + "@" + host)

// PART
# define _PART(user, channel, reason) (user + " PART " + channel + " " + (reason.empty() ? "." : reason ))

#endif