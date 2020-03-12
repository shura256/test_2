#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstring>
#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <socket path>" << std::endl;
    return -1;
  }

  std::string socket_path(argv[1]);
  int rc;
  int fd;
  struct sockaddr_un addr;
  std::array<char, 256> buffer;

  rc = socket(AF_UNIX, SOCK_STREAM, 0);
  if (rc < 0) {
    std::cerr << "error opening socket, " << errno << ", " << strerror(errno)
              << std::endl;
    return -1;
  }

  fd = rc;
  memset(&addr, 0, sizeof(addr));
  strncpy(addr.sun_path, socket_path.c_str(), socket_path.size());
  addr.sun_family = AF_UNIX;

  rc = connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
  if (rc == -1) {
    std::cerr << "error connecting socket, " << errno << ", " << strerror(errno)
              << std::endl;
    return -1;
  }

  while ((rc = read(STDIN_FILENO, buffer.data(), buffer.size())) > 0) {
    if (write(fd, buffer.data(), rc) != rc) {
      std::cerr << "error writing socket, " << errno << ", " << strerror(errno)
                << std::endl;
      return -1;
    }
  }

  return 0;
}
