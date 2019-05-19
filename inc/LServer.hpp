#ifndef L_SERVER_HPP
#define L_SERVER_HPP

#include "lib.h"



class LServer
{
	LServer();

public:
	LServer(std::string	host, int port);
	~LServer();

private:
	boost::asio::io_service			_service;
	boost::asio::ip::tcp::acceptor	_acceptor;

	void					_startWork();
	std::string				_getResponse(std::string request);
	JSON					_get_json_from_request(std::string request, TypeJSON *type);
	TypeJSON				_getType_of_responseJSON(JSON responseJSON);

	std::string				_get_errorHeader();
	std::string 			_get_successHeader(ResultStatus status, int size);
};


#endif