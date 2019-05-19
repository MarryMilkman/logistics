

// #include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "lib.h"

// using namespace boost::asio;
// using boost::system::error_code;

boost::asio::io_service service;

int main(int argc, char* argv[]) {
    boost::asio::ip::tcp::endpoint  ep(boost::asio::ip::address::from_string("127.0.0.1"), 8080);
    boost::asio::ip::tcp::socket    sock(service);
    char                            buf[500000];
    boost::system::error_code       err;
    std::string                     sendbuf;

    while (1) {
        std::string     line;
        std::ifstream   file;
        std::string     header;
        int             bytes;

        sock.connect(ep);
        while (1) {
            getline(std::cin, line);
            if (line == "exit")
                goto exLab;
            line = "../resources/" + line;
            file.open(line);
            if (!file.is_open()) {
                std::cerr << "Can't find file " << line << "\n";
                continue ;
            }
            break ;
        }
        header = "POST / HTTP/1.1\r\nHost: localhost\r\nContent-Type: application/json\r\nContent-Length: ";
        sendbuf = "";
        while (!file.eof()) {
            file >> line;
            sendbuf = sendbuf + line;
        }
        file.close();
        header = header + std::to_string(sendbuf.size()) + "\r\n\r\n";
        sendbuf = header + sendbuf;
        //
        sock.write_some(boost::asio::buffer(sendbuf));
        bytes = read(sock, boost::asio::buffer(buf), boost::asio::transfer_at_least(1), err);
        buf[bytes] = 0;

        std::cerr << "Answer:\n" << buf << "\n";
        sock.close();
    }
exLab:
    return 0;
}