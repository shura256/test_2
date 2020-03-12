#include <iostream>
#include <string>

#include "threadsafe_queue.h"

void do_log(ThreadsafeQueue<std::string>::Ptr log_queue) {
  while (true) {
    std::string log;
    log = log_queue->wait_and_pop();
    std::cout << log << std::endl;
  }
}
