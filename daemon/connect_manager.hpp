#ifndef CONNECT_MANAGER_H
#define CONNECT_MANAGER_H

#include <set>

#include "connection.hpp"

namespace monitord {
namespace server {

/// Keep track of all the current connections and allow us to clean them up when
/// shutting down
class connect_manager
{
public:
  connect_manager(const connect_manager& cm) = delete;
  connect_manager& operator=(const connect_manager& cm) = delete;

  /// Construct the connection manager
  connect_manager() {}

  /// Add and start a new connection
  void add(std::shared_ptr<connection> connection);

  /// Stop and remove a single connection
  void remove(std::shared_ptr<connection> connection);

  /// Stop and remove any active connections
  void shutdown();
private:
  std::set<std::shared_ptr<connection>> cset_;
};

} // namespace server
} // namespace monitord


#endif
