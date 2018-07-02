#include <iostream>

#include "daemon.hpp"

int main()
{
  std::cout << "Daemon starting" << std::endl;

  monitord::server::daemon daemon;

  try {
    daemon.run();
  }
  catch (std::exception& ex)
  {
    std::cerr << "Terminated with exception: " << ex.what() << std::endl;
  }

  return 0;
}
