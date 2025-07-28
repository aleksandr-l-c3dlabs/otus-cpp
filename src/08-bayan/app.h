#pragma once

#include <boost/program_options.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace po = boost::program_options;

class Application {
 public:
  Application(const int argc, const char* argv[]);

  bool is_invalid() const { return !error.empty(); }
  bool is_help() const { return vm.count("help"); }
  void print_invalid_arg(std::ostream& output) const {
    output << error << std::endl << std::endl;
  }
  void print_help(std::ostream& output) const { output << desc << std::endl; }
  std::vector<std::vector<std::string>> diff_groups() const;

 private:
  std::string error;
  po::options_description desc;
  po::variables_map vm;
};