#include "connection.hpp"
#include "connect_manager.hpp"
#include "stats.hpp"

namespace monitord {
namespace server {

void connection::start()
{
  do_read_command();
}

void connection::stop()
{
  socket_.close();
}

void connection::do_read_command()
{
  // make a new reference to store in the closure that async_read_some gets
  auto self(shared_from_this());

  socket_.async_read_some(
    boost::asio::buffer(data_),
    [this, self](boost::system::error_code ec, std::size_t bytes_read) {
      if (!ec) {
        command cmd = parse_command(std::string(data_, bytes_read));
        std::size_t response_length;

        switch (cmd) {

        case command::Unknown: {
          auto response = std::string("Unknown command\n");
          response_length = response.length();
          response.copy(data_, response_length, 0);
          break;
        }

        case command::Cpu: {
          double load = monitor_.get_load();
          std::stringstream ss;
          ss << load << std::endl;
          std::string response(ss.str());

          response_length = response.length();
          response.copy(data_, response_length, 0);

          break;
        }

        case command::Mem: {
          auto response = monitor_.mem_stats();
          response_length = response.length();
          response.copy(data_, response_length, 0);
          break;
        }

        }

        do_write_response(response_length);
      }

      else if (ec != boost::asio::error::operation_aborted) {
        manager_.remove(shared_from_this());
      }
    });
}

void connection::do_write_response(std::size_t bytes)
{
  // make a new reference to store in the closure that async_write gets
  auto self(shared_from_this());

  boost::asio::async_write(
    socket_,
    boost::asio::buffer(data_, bytes),
    [this, self](boost::system::error_code ec, std::size_t) {
      if (!ec) {
        // Terminate connection gracefully after one interaction
        boost::system::error_code err;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
      }
      if (ec != boost::asio::error::operation_aborted) {
        manager_.remove(shared_from_this());
      }
    });
}

command connection::parse_command(const std::string &cmdstring)
{
  if (cmdstring == "mem\n") {
    return command::Mem;
  }

  if (cmdstring == "cpu\n") {
    return command::Cpu;
  }

  return command::Unknown;
}

} // namespace server
} // namespace monitord
