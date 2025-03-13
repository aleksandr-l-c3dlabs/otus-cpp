#include "lib.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <vector>

using ip = std::array<uint8_t, 4>;

std::ostream& operator<<(std::ostream& os, const ip& address) {
  os << static_cast<int>(address[0]) << "." << static_cast<int>(address[1])
     << "." << static_cast<int>(address[2]) << "."
     << static_cast<int>(address[3]);
  return os;
}

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string& str, char d) {
  std::vector<std::string> r;

  std::string::size_type start = 0;
  std::string::size_type stop = str.find_first_of(d);
  while (stop != std::string::npos) {
    r.push_back(str.substr(start, stop - start));

    start = stop + 1;
    stop = str.find_first_of(d, start);
  }

  r.push_back(str.substr(start));

  return r;
}

//
std::vector<ip> filter_ips(const std::vector<ip>& pool,
                           const std::function<bool(const ip&)>& filter) {
  std::vector<ip> result;
  std::copy_if(pool.begin(), pool.end(), std::back_inserter(result), filter);
  return result;
}

//
void print_ips(std::ostream& output, const std::vector<ip>& pool) {
  for (const auto& ip : pool) {
    output << ip << std::endl;
  }
}

//
void ip_filter(std::istream& input, std::ostream& output) {
  std::vector<ip> ip_pool;

  for (std::string line; std::getline(input, line);) {
    auto v = split(line, '\t');
    auto p = split(v.at(0), '.');
    if (p.size() != 4) {
      throw std::invalid_argument("Invalid IP address: " + v.at(0));
    }

    uint8_t parts1 = static_cast<uint8_t>(std::stoi(p[0]));
    uint8_t parts2 = static_cast<uint8_t>(std::stoi(p[1]));
    uint8_t parts3 = static_cast<uint8_t>(std::stoi(p[2]));
    uint8_t parts4 = static_cast<uint8_t>(std::stoi(p[3]));

    ip_pool.push_back({parts1, parts2, parts3, parts4});
  }

  std::sort(ip_pool.begin(), ip_pool.end(),
            [](const ip& a, const ip& b) { return b < a; });

  // all list
  print_ips(output, ip_pool);

  // 1.*.*.*
  print_ips(output, filter_ips(ip_pool, [](const ip& address) {
              return address[0] == 1;
            }));

  // 46.70.*.*
  print_ips(output, filter_ips(ip_pool, [](const ip& address) {
              return address[0] == 46 && address[1] == 70;
            }));

  // 46.*.*.* | *.46.*.* | *.*.46.* | *.*.*.46
  print_ips(output, filter_ips(ip_pool, [](const ip& address) {
              return address[0] == 46 || address[1] == 46 || address[2] == 46 ||
                     address[3] == 46;
            }));
}
