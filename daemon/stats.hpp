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
  cpu_monitor() : prev_total_(0), prev_idle_(0), current_load_(0) {}

  void do_update();
  double get_load();
  std::string mem_stats();
private:
  double prev_total_;
  double prev_idle_;
  double current_load_;
};

} // namespace server
} // namespace monitord

#endif
