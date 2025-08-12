#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "lib.h"

namespace fs = std::filesystem;

using FeaturesRow = std::pair<size_t, Classifier::Features>;

Classifier read_model(const fs::path& path) {
  std::vector<Classifier::Coefficients> coeff_data;

  size_t size = 0;
  std::string line;
  std::ifstream file(path);
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    Classifier::Coefficients coeff;
    if (size > 0)
      coeff.reserve(size);

    float value;
    while (ss >> value) {
      coeff.push_back(value);
    }

    if (size == 0)
      size = coeff.size();
    coeff_data.push_back(coeff);
  }

  return Classifier(coeff_data);
}

std::vector<FeaturesRow> read_csv(const fs::path& path) {
  std::vector<FeaturesRow> result;

  std::string line;
  std::ifstream file(path);
  while (std::getline(file, line)) {
    std::stringstream ss(line);

    Classifier::Features features;
    size_t type;
    float value;
    char comma;

    ss >> type;
    while (ss >> comma >> value) {
      features.push_back(value);
    }

    result.push_back(std::make_pair(type, features));
  }

  return result;
}

int main(const int argc, const char* argv[]) {
  if (argc < 3) {
    std::cerr << "Error: Required filepath parameters (csv data, model file)"
              << std::endl;
    return 1;
  }

  try {
    fs::path csv_file = argv[1];
    fs::path model_file = argv[2];

    if (!fs::exists(csv_file)) {
      std::cerr << "Error: File `" << csv_file.string() << "` is not found"
                << std::endl;
      return 2;
    }

    if (!fs::exists(model_file)) {
      std::cerr << "Error: File `" << model_file.string() << "` is not found"
                << std::endl;
      return 3;
    }

    auto classifier = read_model(model_file);
    uint64_t total = 0, success = 0;
    for (const auto& data : read_csv(csv_file)) {
      auto predict = classifier.predict(data.second);
      if (predict == data.first)
        success++;
      total++;
    }

    std::cout << (static_cast<double>(success) / total) << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 10;
  }

  return 0;
}