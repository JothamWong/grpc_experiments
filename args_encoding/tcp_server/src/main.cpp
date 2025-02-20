#include <asm-generic/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

// Simple TCP server that literally just accepts a TCP connection
int main(int argc, char *argv[]) {
  int port = 8080;
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Socket creation failed" << std::endl;
    return 1;
  }

  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    std::cerr << "Setsockopt failed" << std::endl;
    close(server_fd);
    return 1;
  }

  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    std::cerr << "Bind failed\n";
    close(server_fd);
    return 1;
  }

  if (listen(server_fd, SOMAXCONN) < 0) {
    std::cerr << "Listen failed\n";
    close(server_fd);
    return 1;
  }

  std::cout << "TCP Server listening on port " << port << std::endl;
  while (true) {
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int client_socket =
        accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_socket < 0) {
      std::cerr << "Accept failed" << std::endl;
      continue;
    }
    close(client_socket);
    std::cout << "Accepted and terminated client connection" << std::endl;
  }
  close(server_fd);
  return 0;
}