#include "stats.hpp"

#include <iostream>
#include <fstream>

// The /proc/stat "file" gives an accounting of kernel timeslices ("jiffies")
// spent by each CPU in the system on each of a set of categories of states. The
// values as read are absolute counts since system boot, and the first line
// contains an aggregate total of all the individual CPU entries.

// With such an accounting system, it is impossible to give an immediate
// measurement of CPU load -- the best we can do is some aggregate measurement
// of the percentage of time slices that the CPUs spent in non-idle states. And
// even then, we can not answer immediately about the usage percent over any
// time period except the entire period since boot with a single reading.

// A program such as 'top' remembers the accounting data between screen
// refreshes and can therefore give percentages of time slices spent since the
// last refresh. So, to give anything like an immediate reading of CPU load to
// clients that may request it at arbitrary intervals, we need to calculate it
// independently of client requests and at our own regular intervals; then we
// give the most recently calculated value to clients that request it.

namespace monitord {
namespace server {

std::string cpu_monitor::mem_stats()
{
  return std::string("20547264 - 13409172 - 268620 - 3176212\n");
}

double cpu_monitor::get_load()
{
  return 12.4235;
}

void cpu_monitor::do_update()
{
  std::ifstream ifs("/proc/stat", std::ifstream::in);
  if (!ifs.is_open()) {
    std::cerr << "Internal error: could not open /proc/stat for reading\n";
    return;
  }

  std::string cpu;
  std::size_t user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  ifs >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

  std::cout << user << " " << nice << " " << system << " " << idle << " " << iowait << " " << irq << " "
            << softirq << " " << steal << " " << guest << " " << guest_nice << std::endl;
}

} // namespace server
} // namespace monitord
