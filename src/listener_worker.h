#ifndef LISTENER_WORKER_H_
#define LISTENER_WORKER_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstring>
#include <string>

#include "threadsafe_queue.h"

void do_listen(int port_no, ThreadsafeQueue<std::string>::Ptr data_queue) {
  int sockfd;
  struct sockaddr_in serv_addr = {0};

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "error opening socket, " << errno << ", " << strerror(errno)
              << std::endl;
    return;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port_no);
  if (bind(sockfd, reinterpret_cast<struct sockaddr *>(&serv_addr),
           sizeof(serv_addr)) < 0) {
    std::cerr << "error bind socket, " << errno << ", " << strerror(errno)
              << std::endl;
    return;
  }

  if (listen(sockfd, 5) < 0) {
    std::cerr << "error listen socket, " << errno << ", " << strerror(errno)
              << std::endl;
    return;
  }

  std::array<char, 255> buffer;
  while (true) {
    struct sockaddr_in cli_addr = {0};
    socklen_t cli_len = sizeof(cli_addr);
    int new_sockfd = accept(
        sockfd, reinterpret_cast<struct sockaddr *>(&cli_addr), &cli_len);

    if (new_sockfd < 0) {
      std::cerr << "error accept socket, " << errno << ", " << strerror(errno)
                << std::endl;
    }

    std::string json;
    int read_size = 0;
    do {
      read_size = read(new_sockfd, buffer.data(), buffer.size());
      if (read_size) {
        json.append(buffer.data(), read_size);
      } else if (read_size < 0) {
        std::cerr << "error read socket, " << errno << ", " << strerror(errno)
                  << std::endl;
      }
    } while (read_size > 0);

    if (!json.empty()) data_queue->push(std::move(json));

    close(new_sockfd);
  }

  close(sockfd);
}

#endif  // LISTENER_WORKER_H_
