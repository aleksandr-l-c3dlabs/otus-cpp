#include "lib.h"

double get_var(std::istream& input) {
  double value, avg = 0, n = 0, sum = 0, var = 0;
  while (input >> value) {
    avg = (value + n * avg) / (n + 1);
    var = (value * value + n * var + (n > 0 ? sum * sum / n : 0)) / (n + 1) -
          avg * avg;
    sum += value;
    n++;
  }
  return var;
}