#include "lib.h"
// // #include "LServer.hpp"
// #include "LogisticsController.hpp"
// #include <boost/asio.hpp>

#include "geometry/Dot.hpp"
#include "geometry/Geometry.hpp"
#include "geometry/Line.hpp"


// void	test_geometry() {
	
	
// 	Line	line1(Dot(1, 1), Dot(0, 0));
// 	// Line	line1(Dot(30, 3), Dot(30, 125));
// 	// Dot		dot1(10.5333, 13);

// 	std::cerr << Geometry::get_distance(line1) << "\n";

// 	// try {
// 	// 	Dot dot = Geometry::getDotIntersect(line1, line2);

// 	// 	std::cerr << dot.x << " : " << dot.y << "\n";
// 	// }
// 	// catch (std::exception & e) {
// 	// 	std::cerr << "No intersection\n";
// 	// }
// }

// typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

// //using boost::asio;

// int main(int argc, char const *argv[])
// {
// 	// LServer		server("localhost", 8080);
// 	boost::asio::io_service service;
// 	boost::asio::ip::tcp::endpoint ep( boost::asio::ip::tcp::v4(), 2001); // listen on 2001
// 	boost::asio::ip::tcp::acceptor acc(service, ep);
// 	while ( true) 
// 	{
// 	    socket_ptr sock(new boost::asio::ip::tcp::socket(service));
// 	    acc.accept(*sock);
// 	    boost::thread( boost::bind(client_session, sock));
// 	}
	
// 	return 0;
// }

// void client_session(socket_ptr sock)
// {
// while ( true)
// {
// 	char data[512];
// 	size_t len = sock->read_some(buffer(data));
// 	if ( len > 0) 
// 	write(*sock, buffer("ok", 2));
// }
// }


#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif



#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

io_service service;
size_t read_complete(char * buff, const error_code & err, size_t bytes) {
    if ( err) return 0;
    std::cerr << buff << "\n";
    bool found = std::find(buff, buff + bytes, '\0') < buff + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

void handle_connections() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8080));
    char buff[1024];
    while ( true) {
        ip::tcp::socket sock(service);
        error_code err;

        acceptor.accept(sock);
        std::cerr << "sasat\n";
        int bytes = read(sock, buffer(buff), boost::asio::transfer_at_least(1), err);
        std::cerr << "mmmm?\n";
        std::string msg(buff, bytes);
        sock.write_some(buffer(msg));
        sock.close();
    }
}

int main(int argc, char* argv[]) {
    handle_connections();
    return 0;
}