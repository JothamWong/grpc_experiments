#include "Statistics.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace common {

std::string Statistics::getTimestamp() const {
  auto now = std::chrono::system_clock::now();
  auto now_time = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_time), "%Y%m%d_%H%M%S");
  return ss.str();
}

void Statistics::addMeasurement(double duration) {
  measurements.push_back(duration);
}

double Statistics::getMean() const {
  if (measurements.empty())
    return 0.0;
  double sum = 0.0;
  for (const auto &measurement : measurements) {
    sum += measurement;
  }
  return sum / measurements.size();
}

double Statistics::getStdDev() const {
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

double Statistics::getMin() const {
  if (measurements.empty())
    return 0.0;
  return *std::min_element(measurements.begin(), measurements.end());
}

// Get the maximum execution time
double Statistics::getMax() const {
  if (measurements.empty())
    return 0.0;
  return *std::max_element(measurements.begin(), measurements.end());
}

double Statistics::getMedian() const {
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

size_t Statistics::getSampleSize() const { return measurements.size(); }

void Statistics::printResults(std::string name) const {
  std::cout << "\nBenchmark Results for: " << name << "\n";
  std::cout << "Sample Size: " << Statistics::getSampleSize() << " iterations\n";
  std::cout << "Mean: " << Statistics::getMean() << " ms\n";
  std::cout << "Median: " << Statistics::getMedian() << " ms\n";
  std::cout << "Std Dev: " << Statistics::getStdDev() << " ms\n";
  std::cout << "Min: " << Statistics::getMin() << " ms\n";
  std::cout << "Max: " << Statistics::getMax() << " ms\n";
}

bool Statistics::writeToCSV(const std::string &name) const {
  std::string filename = name + "_" + getTimestamp() + ".csv";
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: could not open file " << filename << " for writing\n";
    return false;
  }

  for (size_t i = 0; i < measurements.size(); i++) {
    file << measurements[i] << "\n";
  }
  file.close();
  std::cout << "Results written to: " << filename << "\n";
  return true;
}
}; // namespace common