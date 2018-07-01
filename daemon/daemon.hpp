#ifndef DAEMON_H
#define DAEMON_H

#include <memory>

#include <boost/asio.hpp>

namespace monitord {
namespace server {

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