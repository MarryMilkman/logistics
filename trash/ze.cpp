


#include <iostream>
#include <vector>

#include <fstream>
#include <iomanip>
// #include <nlohmann/json.hpp>
// #include <libpq-fe.h>
#include <cmath>

// #include <WinSock2.h>
// #include <WS2tcpip.h>

// #pragma comment(lib, "Ws2_32.lib")


enum    IntersectionType {
    PolygonInclude = 0,
    PolygonUpper,
    Intersection_true,
    Intersection_false,
    FullMatch,
    ErrorPartsOfOneArya
};

enum    RelativePosition {
    Outside = 0,
    Inside,
    Border
};

enum    TypeJSON {
    tPolygon,
    tPolyline,
    tError
}

struct s
{
    int s;
};

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}



// void client_session(socket_ptr sock)
// {
// while ( true)
// {
//  char data[512];
//  size_t len = sock->read_some(buffer(data));
//  if ( len > 0) 
//  write(*sock, buffer("ok", 2));
// }
// }


// #ifdef WIN32
// #define _WIN32_WINNT 0x0501
// #include <stdio.h>
// #endif



// #include <boost/bind.hpp>
// #include <boost/asio.hpp>
// #include <boost/shared_ptr.hpp>
// #include <boost/enable_shared_from_this.hpp>
// using namespace boost::asio;
// using namespace boost::posix_time;
// using boost::system::error_code;

// io_service service;
// // size_t read_complete(char * buff, const error_code & err, size_t bytes) {
// //     if ( err) return 0;
// //     std::cerr << buff << "\n";
// //     bool found = std::find(buff, buff + bytes, '\0') < buff + bytes;
// //     // we read one-by-one until we get to enter, no buffering
// //     return found ? 0 : 1;
// // }

// void handle_connections() {
    // ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8080));
//     char buff[1024];
//     while ( true) {
//         ip::tcp::socket  sock(service);
//         error_code           err;

//         acceptor.accept(sock);
//         std::cerr << "sasat\n";
//         int bytes = read(sock, buffer(buff), boost::asio::transfer_at_least(1), err);
//         std::cerr << "mmmm?\n";
//         std::string msg(buff, bytes);
//         sock.write_some(buffer(msg));
//         sock.close();
//     }
// }

// int main(int argc, char* argv[]) {
//     handle_connections();
//     return 0;
// }


CREATE TABLE IF NOT EXISTS table_polygons(
 arr_dot json NOT NULL,
 color text NOT NULL,
 id_polygon text NOT NULL
);




















