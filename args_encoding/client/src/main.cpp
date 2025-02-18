#include "Statistics.hpp"

#include <ctime>
#include <functional>
#include <ios>
#include <myproto/message.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

// struct Statistics {
//     double
// };

#define NUM_ITERATIONS 1000
// #define NUM_ITERATIONS 100000

template <typename RequestType, typename ResponseType>
void benchmark_grpc_call(
    const std::string &name,
    std::function<grpc::Status(grpc::ClientContext *, const RequestType &,
                               ResponseType *)>
        grpc_call,
    std::function<void(RequestType &)> setup_request,
    std::unique_ptr<protobenchmark::Messages::Stub> &stub, int num_iterations) {
  std::cout << "Benchmarking " << name << std::endl;
  common::Statistics stats;

  for (int i = 0; i < num_iterations; i++) {
    std::cout << "Iteration " << i << std::endl;
    grpc::ClientContext context;
    RequestType request;
    ResponseType response;
    setup_request(request);
    auto t1 = high_resolution_clock::now();
    grpc::Status status = grpc_call(&context, request, &response);
    auto t2 = high_resolution_clock::now();

    double duration =
        std::chrono::duration<double, std::milli>(t2 - t1).count();
    stats.addMeasurement(duration);
    if (!status.ok()) {
      std::cerr << "Failed gRPC call for " << name << std::endl;
    }
  }
  stats.printResults(name);
  stats.writeToCSV(name);
}

int main(int argc, char *argv[]) {
  std::cout << "Client starting!" << std::endl;
  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());
  std::unique_ptr<protobenchmark::Messages::Stub> stub =
      protobenchmark::Messages::NewStub(channel);
  std::cout << "Num iterations: " << NUM_ITERATIONS << std::endl;

  benchmark_grpc_call<google::protobuf::Empty, google::protobuf::Empty>(
      "Zero Arg Benchmark",
      [&stub](grpc::ClientContext *context,
              const google::protobuf::Empty &request,
              google::protobuf::Empty *response) {
        return stub->GetM0(context, request, response);
      },
      [](google::protobuf::Empty &request) {}, stub, NUM_ITERATIONS);

  benchmark_grpc_call<protobenchmark::M1, protobenchmark::M1>(
      "One Arg Benchmark",
      [&stub](grpc::ClientContext *context, const protobenchmark::M1 &request,
              protobenchmark::M1 *response) {
        return stub->GetM1(context, request, response);
      },
      [](protobenchmark::M1 &request) { request.set_arg1(0); }, stub,
      NUM_ITERATIONS);

  benchmark_grpc_call<protobenchmark::M2, protobenchmark::M2>(
      "Two Arg Benchmark",
      [&stub](grpc::ClientContext *context, const protobenchmark::M2 &request,
              protobenchmark::M2 *response) {
        return stub->GetM2(context, request, response);
      },
      [](protobenchmark::M2 &request) {
        request.set_arg1(0);
        request.set_arg2(0);
      },
      stub, NUM_ITERATIONS);

  benchmark_grpc_call<protobenchmark::M3, protobenchmark::M3>(
      "Three Arg Benchmark",
      [&stub](grpc::ClientContext *context, const protobenchmark::M3 &request,
              protobenchmark::M3 *response) {
        return stub->GetM3(context, request, response);
      },
      [](protobenchmark::M3 &request) {
        request.set_arg1(0);
        request.set_arg2(0);
        request.set_arg3(0);
      },
      stub, NUM_ITERATIONS);

  return 0;
}