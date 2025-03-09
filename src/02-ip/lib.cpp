#include "lib.h"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

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
struct ip {
  uint8_t parts1;
  uint8_t parts2;
  uint8_t parts3;
  uint8_t parts4;

  ip(const std::string& str) {
    auto p = split(str, '.');
    if (p.size() != 4) {
      throw std::invalid_argument("Invalid IP address: " + str);
    }

    parts1 = static_cast<uint8_t>(std::stoi(p[0]));
    parts2 = static_cast<uint8_t>(std::stoi(p[1]));
    parts3 = static_cast<uint8_t>(std::stoi(p[2]));
    parts4 = static_cast<uint8_t>(std::stoi(p[3]));
  }

  bool operator==(const ip& other) const {
    return parts1 == other.parts1 && parts2 == other.parts2 &&
           parts3 == other.parts3 && parts4 == other.parts4;
  }

  bool operator<(const ip& other) const {
    if (parts1 != other.parts1)
      return parts1 < other.parts1;
    if (parts2 != other.parts2)
      return parts2 < other.parts2;
    if (parts3 != other.parts3)
      return parts3 < other.parts3;
    return parts4 < other.parts4;
  }

  friend std::ostream& operator<<(std::ostream& os, const ip& ip) {
    os << static_cast<int>(ip.parts1) << "." << static_cast<int>(ip.parts2)
       << "." << static_cast<int>(ip.parts3) << "."
       << static_cast<int>(ip.parts4);
    return os;
  }
};

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
    ip_pool.push_back(ip(v.at(0)));
  }

  std::sort(ip_pool.begin(), ip_pool.end(),
            [](const ip& a, const ip& b) { return b < a; });

  // all list
  print_ips(output, ip_pool);

  // 1.*.*.*
  print_ips(output, filter_ips(ip_pool, [](const ip& address) {
              return address.parts1 == 1;
            }));

  // 46.70.*.*
  print_ips(output, filter_ips(ip_pool, [](const ip& address) {
              return address.parts1 == 46 && address.parts2 == 70;
            }));

  // 46.*.*.* | *.46.*.* | *.*.46.* | *.*.*.46
  print_ips(output, filter_ips(ip_pool, [](const ip& address) {
              return address.parts1 == 46 || address.parts2 == 46 ||
                     address.parts3 == 46 || address.parts4 == 46;
            }));
}
