#pragma once

#include <string>
#include <vector>

namespace common {

class Statistics {
private:
  std::vector<double> measurements;
  std::string getTimestamp() const;

public:
  void addMeasurement(double duration);

  double getMean() const;

  double getStdDev() const;

  double getMin() const;

  double getMax() const;

  double getMedian() const;

  size_t getSampleSize() const;

  void printResults(std::string name) const;

  bool writeToCSV(const std::string &name) const;
};

} // namespace common