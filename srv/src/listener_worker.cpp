#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

#include "threadsafe_queue.h"

void do_listen(const std::string &socket_path,
               ThreadsafeQueue<std::string>::Ptr data_queue) {
  int sockfd;
  struct sockaddr_un serv_addr;

  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "error opening socket, " << errno << ", " << strerror(errno)
              << std::endl;
    return;
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  strncpy(serv_addr.sun_path, socket_path.c_str(), socket_path.size());
  serv_addr.sun_family = AF_UNIX;
  unlink(socket_path.c_str());
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
    int cli_sockfd = accept(sockfd, NULL, NULL);

    if (cli_sockfd < 0) {
      std::cerr << "error accept socket, " << errno << ", " << strerror(errno)
                << std::endl;
      continue;
    }

    std::string json;
    int read_count;
    while ((read_count = read(cli_sockfd, buffer.data(), buffer.size())) > 0) {
      json.append(buffer.data(), read_count);
    }

    if (!json.empty()) data_queue->push(std::move(json));

    close(cli_sockfd);
  }

  close(sockfd);
}
