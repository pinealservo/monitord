#include "connection.hpp"
#include "connect_manager.hpp"

namespace monitord {
namespace server {

void connection::start()
{
  // make a new reference to store in the closure that async_write gets
  auto self(shared_from_this());

  boost::asio::async_write(
    socket_,
    boost::asio::buffer(data_),
    [this, self](boost::system::error_code ec, std::size_t) {
      if (!ec)
        manager_.remove(self);
      });
}

void connection::stop()
{
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
}

} // namespace server
} // namespace monitord
