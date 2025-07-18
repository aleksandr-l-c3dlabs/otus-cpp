#include "lib.h"

#include <string>

bool get_column(std::istream& input, std::ostream& output,
                const u_int8_t column, const char splitter) {
  std::string line, part;
  std::stringstream linestream(line);
  u_int8_t current = 0;
  while (std::getline(input, line)) {
    current = 0;
    linestream.str(line);
    linestream.clear();
    linestream.seekg(0);
    linestream.seekp(0);

    while (std::getline(linestream, part, splitter)) {
      if (current == column) {
        output << part << std::endl;
        break;
      } else {
        current++;
      }
    }

    if (current != column)
      return false;
  }

  return true;
}