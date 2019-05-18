

// #include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "lib.h"

// using namespace boost::asio;
// using boost::system::error_code;
boost::asio::io_service service;

size_t read_complete(char * buf, const boost::system::error_code & err, size_t bytes) {
    if ( err) return 0;
    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

boost::asio::ip::tcp::endpoint ep( boost::asio::ip::address::from_string("127.0.0.1"), 8080);
void sync_echo(std::string msg) {
    msg += "\n";
    boost::asio::ip::tcp::socket sock(service);
    
    sock.connect(ep);
    sock.write_some(boost::asio::buffer(msg));
    char buf[1024];
    int bytes = read(sock, boost::asio::buffer(buf), boost::bind(read_complete,buf,_1,_2));
    std::string copy(buf, bytes - 1);
    msg = msg.substr(0, msg.size() - 1);
    std::cout << "server echoed our " << msg << ": "
                << (copy == msg ? "OK" : "FAIL") << std::endl;
    std::cerr << "sam5\n";
    sock.close();
}

int main(int argc, char* argv[]) {
    // connect several clients
    char* messages[] = { "John says hi", 0 };
    for ( char ** message = messages; *message; ++message) {
        sync_echo(*message);
    }
}