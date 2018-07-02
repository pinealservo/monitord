#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>

#include <boost/asio.hpp>

namespace monitord {
namespace server {

// Forward declare the manager class; we need to keep a reference to it so we can
// remove connections when they close normally
class connect_manager;
class cpu_monitor;

// The commands that the server recognizes
enum class command { Unknown, Cpu, Mem };

/// Class for managing connections to the daemon server
class connection : public std::enable_shared_from_this<connection>
{
public:

  // Don't want default copy or assignment constructors.
  connection(const connection&) = delete;
  connection& operator=(const connection &) = delete;

  /// Construct a new connection from its socket
  explicit connection(boost::asio::ip::tcp::socket sock,
                      connect_manager& manager,
                      cpu_monitor& monitor) :
    socket_(std::move(sock)),
    manager_(manager),
    monitor_(monitor)
  {
  }

  /// Run the socket processing
  void start();

  /// Close and clean up the socket
  void stop();

private:
  /// Read an incoming command from the socket
  void do_read_command();

  /// Write the response to the socket
  void do_write_response(std::size_t bytes);

  /// Parse a command
  command parse_command(const std::string &cmdstring);

  boost::asio::ip::tcp::socket socket_;
  connect_manager& manager_;
  cpu_monitor& monitor_;

  enum { MAX_LENGTH = 1024 };
  char data_[MAX_LENGTH];
};


} // namespace server
} // namespace monitord

#endif
