#include "lib.h"

#include <cassert>
#include <iostream>
#include <tuple>

int main() {
  try {
    Matrix<int, 0> matrix;

    // заполнить главную диагональ (от [0,0] до [9,9]) значениями от 0 до 9
    for (int i = 0; i < 10; ++i) {
      matrix[i][i] = i;
    }

    // заполнить второстепенную диагональ (от [0,0] до [9,9]) значениями от 9 до 0
    for (int i = 9; i > -1; --i) {
      matrix[9 - i][i] = i;
    }

    // вывести фрагмент мтарицы
    for (int i = 1; i < 9; ++i) {
      for (int j = 1; j < 9; ++j) {
        std::cout << matrix[i][j] << (j < 8 ? " " : "");
      }
      std::cout << std::endl;
    }

    // вывести количество занятых ячеек
    std::cout << matrix.size() << std::endl;
    assert(matrix.size() == 18);

    // вывести все занятые ячейки вместе со своими позициями
    for (auto c : matrix) {
      int x, y, v;
      std::tie(x, y, v) = c;
      std::cout << '[' << x << "][" << y << "] = " << v << std::endl;
    }
    // каноническая форма оператора =
    ((matrix[100][100] = 314) = 0) = 217;
    std::cout << matrix[100][100] << std::endl;

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}