#include "stats.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>

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

/*
Algorithm description copied from:
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux/

PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
*/

namespace monitord {
namespace server {

std::string cpu_monitor::mem_stats()
{
  std::ifstream ifs("/proc/meminfo", std::ifstream::in);
  if (!ifs.is_open()) {
    return std::string("Internal error: could not open /proc/meminfo for reading\n");
  }

  std::string stat_name;
  std::size_t max_ignore, mem_total, mem_free, buffers, cached;
  max_ignore = std::numeric_limits<std::streamsize>::max();

  // For each line we are interested in, read the stat name and value, then skip to the next line
  // For lines we aren't interested in, skip the whole line

  // MemTotal
  ifs >> stat_name >> mem_total;
  ifs.ignore(max_ignore,'\n');

  // MemFree
  ifs >> stat_name >> mem_free;
  ifs.ignore(max_ignore,'\n');

  // MemAvailable - skip
  ifs.ignore(max_ignore,'\n');

  // Buffers
  ifs >> stat_name >> buffers;
  ifs.ignore(max_ignore,'\n');

  // Cached
  ifs >> stat_name >> cached;

  // Format specified as: MemTotal - MemFree - Buffers - Cached
  std::stringstream ss;
  ss << mem_total << " - " << mem_free << " - " << buffers << " - " << cached << std::endl;
  return ss.str();
}

double cpu_monitor::get_load()
{
  return current_load_;
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

  // Read the first line (aggregate counts) of values in
  ifs >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

  double cpu_idle = idle + iowait;
  double cpu_total = idle + iowait + user + nice + system + irq + softirq + steal;
  double idle_diff = cpu_idle - prev_idle_;
  double total_diff = cpu_total - prev_total_;

  prev_idle_ = cpu_idle;
  prev_total_ = cpu_total;

  current_load_ = (total_diff - idle_diff) / total_diff;
}

} // namespace server
} // namespace monitord
