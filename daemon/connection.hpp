#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>

#include <boost/asio.hpp>

namespace monitord {
namespace server {

/// Class for managing connections to the daemon server
class connection : public std::enable_shared_from_this<connection>
{
public:

  // Don't want default copy or assignment constructors.
  connection(const connection&) = delete;
  connection& operator=(const connection &) = delete;

  /// Construct a new connection from its socket
  explicit connection(boost::asio::ip::tcp::socket sock) :
    socket_(std::move(sock)),
    data_{"Hello, World!\n"} {};

  /// Run the socket processing
  void start();

private:

  boost::asio::ip::tcp::socket socket_;

  enum { MAX_LENGTH = 256 };
  char data_[MAX_LENGTH];
};


} // namespace server
} // namespace monitord

#endif
