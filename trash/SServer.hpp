#ifndef S_SERVER_HPP
# define S_SERVER_HPP

# include "lib.h"

class SServer
{
public:
	SServer();
	~SServer();
	void startServer();
	void closeServer();
	void handle();
	unsigned short port;
private:
	SOCKET this_s;
	WSAData wData;
};

#endif