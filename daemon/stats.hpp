#ifndef STATS_H
#define STATS_H

#include <string>

namespace monitord {
namespace server {

class cpu_monitor
{
public:
  cpu_monitor(const cpu_monitor&) = delete;
  cpu_monitor& operator=(const cpu_monitor&) = delete;
  cpu_monitor() {}

  void do_update();
  double get_load();
  std::string mem_stats();
private:
  std::size_t prev_total_;
  std::size_t prev_idle_;
};

} // namespace server
} // namespace monitord

#endif
