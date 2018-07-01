#ifndef DAEMON_H
#define DAEMON_H

#include <boost/asio.hpp>

#include <memory>

namespace monitord {
namespace server {

/// Class for managing connections to the daemon server
class connection : public std::enable_shared_from_this<connection>
{
public:
  explicit connection(boost::asio::ip::tcp::socket sock) :
    socket_(std::move(sock)),
    data_{"Hello, World!\n"} {};
  void start();
private:
  boost::asio::ip::tcp::socket socket_;
  enum { MAX_LENGTH = 256 };
  char data_[MAX_LENGTH];
};

/// Top-level class for the monitor daemon server component
class daemon
{
public:

  /// Construct the daemon, initializing the asio subsystem and begin accepting
  daemon();

  /// Run the server's io_service loop until it exits
  void run();

private:

  /// Repeatedly asynchronously accept new connections
  void do_accept();

  /// Register an asynchronous await for reception of an OS signal
  void do_await_signal();

  boost::asio::io_service io_service_;
  boost::asio::signal_set signal_set_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
};

} // namespace server
} // namespace monitord

#endif
