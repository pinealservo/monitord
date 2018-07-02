#include <iostream>
#include <boost/asio.hpp>

#include <signal.h>

#define DAEMON_PORT "5001"

using namespace boost::asio::ip;

int main(int argc, char *argv[])
{
  try {
    if (argc != 2) {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    std::cout << "Connecting to daemon on " << argv[1] << std::endl;

    boost::asio::io_service ios;

    tcp::resolver resolver(ios);
    tcp::resolver::query q(argv[1], DAEMON_PORT);
    tcp::resolver::iterator it = resolver.resolve(q);

    tcp::socket sock(ios);

    for (;;) {
      char buf[128];
      std::string cmd;
      boost::system::error_code error;

      // Read a command
      std::cin >> cmd;
      cmd += "\n";

      // Make the connection
      boost::asio::connect(sock, it);

      // Send it over
      boost::asio::write(sock, boost::asio::buffer(cmd), error);

      // Read the response
      std::size_t len = sock.read_some(boost::asio::buffer(buf), error);

      // Print the response
      std::cout << std::string(buf, len);
    }

  }
  catch (std::exception ex) {
    std::cerr << "Terminated with exception: " << ex.what() << std::endl;
  }

  return 0;
}
