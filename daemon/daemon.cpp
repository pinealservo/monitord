#include <iostream>

#include <signal.h>

#include "daemon.hpp"
#include "connection.hpp"

#define DAEMON_PORT 5001

namespace monitord {
namespace server {


daemon::daemon(void)
  : io_service_(),
    signal_set_(io_service_),
    timer_(io_service_),
    acceptor_(io_service_),
    socket_(io_service_)
{
  // Register signals so we can terminate cleanly
  signal_set_.add(SIGINT);
  signal_set_.add(SIGTERM);

  // Add a signal await async operation to the io_service
  do_await_signal();

  // Start monitoring cpu stats
  timer_.expires_at(std::chrono::steady_clock::now());
  do_update_stats();

  // Set up the asio TCP listening socket

  boost::asio::ip::tcp::endpoint tcp_endpoint{boost::asio::ip::tcp::v4(), DAEMON_PORT};
  acceptor_.open(tcp_endpoint.protocol());
  // set reuse_address option so we don't have to wait between restarts
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(tcp_endpoint);
  acceptor_.listen();

  do_accept();
}

void daemon::run()
{
  io_service_.run();
}

void daemon::do_accept()
{
  static const std::string greeting{"Hello, World!\n"};

  std::cout << "Waiting for a new connection..." << std::endl;

  acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
      // Return immediately if the signal handler closed the acceptor; this will end the daemon
      if (!acceptor_.is_open()) {
        return;
      }

      // Successful accept
      if (!ec) {

        std::cout << "Accepted a connection!" << std::endl;

        // Use a shared pointer to keep the connection object allocated until references are gone
        manager_.add(std::make_shared<connection>(std::move(socket_), manager_, monitor_));
      }

      // invoke ourselves recursively to continue accepting
      do_accept();
    });
}

void daemon::do_await_signal()
{
  signal_set_.async_wait([this](boost::system::error_code, int) {
      // We got a signal, so stop accepting new connections
      acceptor_.close();
      // Cancel the periodic monitor timer
      timer_.expires_from_now(std::chrono::seconds(0));

      std::cout << "Shutting down due to signal" << std::endl;

      manager_.shutdown();
    });
}

void daemon::do_update_stats()
{
  timer_.expires_at(timer_.expires_at() + std::chrono::seconds(1));
  timer_.async_wait([this](boost::system::error_code ec) {
      if (!ec) {
        monitor_.do_update();
        do_update_stats();
      }
    });
}

} // namespace server
} // namespace monitord
