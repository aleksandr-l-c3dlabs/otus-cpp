#include "lib.h"

double get_avg(std::istream& input) {
  double value, avg = 0, n = 0;
  while (input >> value) {
    avg = (value + n * avg) / (n + 1);
    n++;
  }
  return avg;
}