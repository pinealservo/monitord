#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>

#include <boost/asio.hpp>

namespace monitord {
namespace server {

// Forward declare the manager class; we need to keep a reference to it so we can
// remove connections when they close normally
class connect_manager;

/// Class for managing connections to the daemon server
class connection : public std::enable_shared_from_this<connection>
{
public:

  // Don't want default copy or assignment constructors.
  connection(const connection&) = delete;
  connection& operator=(const connection &) = delete;

  /// Construct a new connection from its socket
  explicit connection(boost::asio::ip::tcp::socket sock,
                      connect_manager& manager) :
    socket_(std::move(sock)),
    manager_(manager),
    data_{"Hello, World!\n"} {};

  /// Run the socket processing
  void start();

  /// Close and clean up the socket
  void stop();

private:

  boost::asio::ip::tcp::socket socket_;
  connect_manager& manager_;

  enum { MAX_LENGTH = 256 };
  char data_[MAX_LENGTH];
};


} // namespace server
} // namespace monitord

#endif
