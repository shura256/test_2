#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "listener_worker.h"
#include "log_worker.h"
#include "threadsafe_queue.h"
#include "worker.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <port number>" << std::endl;
    return -1;
  }

  int port_no = std::stoi(argv[1]);
  auto data_queue = std::make_shared<ThreadsafeQueue<std::string>>();
  auto log_queue = std::make_shared<ThreadsafeQueue<std::string>>();
  std::vector<std::thread> threads;

  threads.emplace_back(do_listen, port_no, data_queue);
  threads.emplace_back(do_log, log_queue);

  int worker_count = std::thread::hardware_concurrency() - threads.size();
  if (worker_count < 0) worker_count = 2;

  for (int i = 0; i < worker_count; ++i)
    threads.emplace_back(do_work, data_queue, log_queue);

  for (std::size_t i = 0; i < threads.size(); ++i) {
    if (threads[i].joinable()) threads[i].join();
  }

  return 0;
}
