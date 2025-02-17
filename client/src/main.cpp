#include <ctime>
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

#define NUM_ITERATIONS 100000

class Statistics {
private:
  std::vector<double> measurements;

  std::string getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%Y%m%d_%H%M%S");
    return ss.str();
  }

public:
  void addMeasurement(double duration) { measurements.push_back(duration); }

  double getMean() const {
    if (measurements.empty())
      return 0.0;
    double sum = 0.0;
    for (const auto &measurement : measurements) {
      sum += measurement;
    }
    return sum / measurements.size();
  }

  double getStdDev() const {
    if (measurements.size() < 2)
      return 0.0;
    double mean = getMean();
    double sumSquaredDiff = 0.0;

    for (const auto &measurement : measurements) {
      double diff = measurement - mean;
      sumSquaredDiff += diff * diff;
    }

    return std::sqrt(sumSquaredDiff / (measurements.size() - 1));
  }

  double getMin() const {
    if (measurements.empty())
      return 0.0;
    return *std::min_element(measurements.begin(), measurements.end());
  }

  // Get the maximum execution time
  double getMax() const {
    if (measurements.empty())
      return 0.0;
    return *std::max_element(measurements.begin(), measurements.end());
  }

  double getMedian() const {
    if (measurements.empty())
      return 0.0;

    std::vector<double> sorted = measurements;
    std::sort(sorted.begin(), sorted.end());

    if (sorted.size() % 2 == 0) {
      return (sorted[sorted.size() / 2 - 1] + sorted[sorted.size() / 2]) / 2.0;
    } else {
      return sorted[sorted.size() / 2];
    }
  }

  size_t getSampleSize() const { return measurements.size(); }

  void printResults(std::string name) const {
    std::cout << "\nBenchmark Results for: " << name << "\n";
    std::cout << "Sample Size: " << getSampleSize() << " iterations\n";
    std::cout << "Mean: " << getMean() << " ms\n";
    std::cout << "Median: " << getMedian() << " ms\n";
    std::cout << "Std Dev: " << getStdDev() << " ms\n";
    std::cout << "Min: " << getMin() << " ms\n";
    std::cout << "Max: " << getMax() << " ms\n";
  }

  bool writeToCSV(const std::string &name) const {
    std::string filename = name + "_" + getTimestamp() + ".csv";
    std::ofstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: could not open file " << filename
                << " for writing\n";
      return false;
    }

    for (size_t i = 0; i < measurements.size(); i++) {
      file << measurements[i] << "\n";
    }
    file.close();
    std::cout << "Results written to: " << filename << "\n";
    return true;
  }
};

// void benchmark_no_arguments(std::unique_ptr<protobenchmark::Messages::Stub>&
// stub, int num_iterations) {
//     for (int i = 0; i < num_iterations; ++i) {
//         grpc::ClientContext context;
//         auto t1 = high_resolution_clock::now();
//         grpc::Status status = stub->GetM0(&context);
//         auto t2 = high_resolution_clock::now();
//     }
//     return;
// }
void benchmark_zero_arguments(
    std::unique_ptr<protobenchmark::Messages::Stub> &stub, int num_iterations) {
  std::cout << "Benchmarking zero argument" << std::endl;
  Statistics stats;
  for (int i = 0; i < num_iterations; ++i) {
    // std::cout << "Iteration " << i << std::endl;
    grpc::ClientContext context;
    google::protobuf::Empty query;
    google::protobuf::Empty result;
    auto t1 = high_resolution_clock::now();
    grpc::Status status = stub->GetM0(&context, query, &result);
    auto t2 = high_resolution_clock::now();
    double duration =
        std::chrono::duration<double, std::milli>(t2 - t1).count();
    stats.addMeasurement(duration);
    // std::cout << "Result successfully modified " << result.arg1() <<
    // std::endl;
  }
  stats.printResults("Zero Arg Benchmark");
  stats.writeToCSV("Zero Arg Benchmark");
  return;
}

void benchmark_one_arguments(
    std::unique_ptr<protobenchmark::Messages::Stub> &stub, int num_iterations) {
  std::cout << "Benchmarking one argument" << std::endl;
  Statistics stats;
  for (int i = 0; i < num_iterations; ++i) {
    // std::cout << "Iteration " << i << std::endl;
    grpc::ClientContext context;
    protobenchmark::M1 query;
    protobenchmark::M1 result;
    query.set_arg1(0);
    auto t1 = high_resolution_clock::now();
    grpc::Status status = stub->GetM1(&context, query, &result);
    auto t2 = high_resolution_clock::now();
    double duration =
        std::chrono::duration<double, std::milli>(t2 - t1).count();
    stats.addMeasurement(duration);
    // std::cout << "Result successfully modified " << result.arg1() <<
    // std::endl;
  }
  stats.printResults("One Arg Benchmark");
  stats.writeToCSV("One Arg Benchmark");
  return;
}

void benchmark_two_arguments(
    std::unique_ptr<protobenchmark::Messages::Stub> &stub, int num_iterations) {
  std::cout << "Benchmarking one argument" << std::endl;
  Statistics stats;
  for (int i = 0; i < num_iterations; ++i) {
    // std::cout << "Iteration " << i << std::endl;
    grpc::ClientContext context;
    protobenchmark::M2 query;
    protobenchmark::M2 result;
    query.set_arg1(0);
    query.set_arg2(0);
    auto t1 = high_resolution_clock::now();
    grpc::Status status = stub->GetM2(&context, query, &result);
    auto t2 = high_resolution_clock::now();
    double duration =
        std::chrono::duration<double, std::milli>(t2 - t1).count();
    stats.addMeasurement(duration);
    // std::cout << "Result successfully modified " << result.arg1() <<
    // std::endl;
  }
  stats.printResults("Two Arg Benchmark");
  stats.writeToCSV("Two Arg Benchmark");
  return;
}

void benchmark_three_arguments(
    std::unique_ptr<protobenchmark::Messages::Stub> &stub, int num_iterations) {
  std::cout << "Benchmarking one argument" << std::endl;
  Statistics stats;
  for (int i = 0; i < num_iterations; ++i) {
    // std::cout << "Iteration " << i << std::endl;
    grpc::ClientContext context;
    protobenchmark::M3 query;
    protobenchmark::M3 result;
    query.set_arg1(0);
    query.set_arg2(0);
    query.set_arg3(0);
    auto t1 = high_resolution_clock::now();
    grpc::Status status = stub->GetM3(&context, query, &result);
    auto t2 = high_resolution_clock::now();
    double duration =
        std::chrono::duration<double, std::milli>(t2 - t1).count();
    stats.addMeasurement(duration);
    // std::cout << "Result successfully modified " << result.arg1() <<
    // std::endl;
  }
  stats.printResults("Three Arg Benchmark");
  stats.writeToCSV("Three Arg Benchmark");
  return;
}

int main(int argc, char *argv[]) {
  std::cout << "Client starting!" << std::endl;
  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());
  std::unique_ptr<protobenchmark::Messages::Stub> stub =
      protobenchmark::Messages::NewStub(channel);
  std::cout << "Num iterations: " << NUM_ITERATIONS << std::endl;
  benchmark_zero_arguments(stub, NUM_ITERATIONS);
  benchmark_one_arguments(stub, NUM_ITERATIONS);
  benchmark_two_arguments(stub, NUM_ITERATIONS);
  benchmark_three_arguments(stub, NUM_ITERATIONS);

  // Output result
  // std::cout << "I got:" << std::endl;
  // std::cout << "Name: " << result.name() << std::endl;
  // std::cout << "City: " << result.city() << std::endl;
  // std::cout << "Zip:  " << result.zip() << std::endl;
  // std::cout << "Street: " << result.street() << std::endl;
  // std::cout << "Country: " << result.country() << std::endl;

  return 0;
}