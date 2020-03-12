#include <chrono>
#include <string>
#include <thread>

#include "threadsafe_queue.h"

using namespace std::chrono_literals;

void do_work(ThreadsafeQueue<std::string>::Ptr data_queue,
             ThreadsafeQueue<std::string>::Ptr log_queue) {
  while (true) {
    std::string json;
    json = data_queue->wait_and_pop();
    std::this_thread::sleep_for(5s);
    log_queue->push("json\n" + json + "\nprocessed");
  }
}
