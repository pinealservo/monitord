#include "connect_manager.hpp"
#include "connection.hpp"

namespace monitord {
namespace server {

void connect_manager::add(std::shared_ptr<connection> connection)
{
  cset_.insert(connection);
  connection->start();
}

void connect_manager::remove(std::shared_ptr<connection> connection)
{
  connection->stop();
  cset_.erase(connection);
}

void connect_manager::shutdown()
{
  for (auto connection : cset_) {
    connection->stop();
  }
  cset_.clear();
}

} // namespace server
} // namespace monitord
