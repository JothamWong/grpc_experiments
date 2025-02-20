#include "Statistics.hpp"

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <myproto/message.grpc.pb.h>

#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define NUM_ITERATIONS 100000
#define PORT 8080

void measure_grpc_overhead(int iterations) {
  common::Statistics stats;
  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());
  std::unique_ptr<protobenchmark::Messages::Stub> stub =
      protobenchmark::Messages::NewStub(channel);
  for (int i = 0; i < iterations; i++) {
    auto t1 = high_resolution_clock::now();
    grpc::ClientContext context;
    google::protobuf::Empty request;
    google::protobuf::Empty response;
    grpc::Status status = stub->GetM0(&context, request, &response);
    auto t2 = high_resolution_clock::now();
    double duration =
        std::chrono::duration<double, std::milli>(t2 - t1).count();
    stats.addMeasurement(duration);
    if (!status.ok()) {
      std::cerr << "Failed gRPC call" << std::endl;
    }
  }
  stats.printResults("gRPC overhead");
  stats.writeToCSV("gRPC overhead");
}

void measure_tcp_overhead(int iterations) {
  common::Statistics stats;
  for (int i = 0; i < iterations; i++) {
    auto t1 = high_resolution_clock::now();
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      continue;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    int result =
        connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    close(sock);

    auto t2 = high_resolution_clock::now();
    double duration =
        std::chrono::duration<double, std::milli>(t2 - t1).count();
    stats.addMeasurement(duration);
    if (result == -1) {
      std::cerr << "Error establishing TCP connection" << std::endl;
    }
  }
  stats.printResults("TCP overhead");
  stats.writeToCSV("TCP overhead");
}

int main(int argc, char *argv[]) {
  std::cout << "Client starting!" << std::endl;
  measure_grpc_overhead(NUM_ITERATIONS);
  measure_tcp_overhead(NUM_ITERATIONS);
  return 0;
}