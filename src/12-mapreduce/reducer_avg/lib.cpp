#include "lib.h"

double get_avg(std::istream& input) {
  long double value, avg = 0, n = 0;
  while (input >> value) {
    avg += (value - avg) / (n + 1);
    n++;
  }
  return avg;
}