#include "connection.hpp"
#include "connect_manager.hpp"

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
        // @todo Parse command here
        // For now, we'll echo back what was sent
        do_write_response(bytes_read);
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

} // namespace server
} // namespace monitord
