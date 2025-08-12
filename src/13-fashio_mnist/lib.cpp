#include "lib.h"

#include <algorithm>

Classifier::Classifier(const std::vector<Coefficients>& matrix_data) {

  size_t rows = matrix_data.size();
  size_t cols = matrix_data.empty() ? 0 : matrix_data[0].size();

  _model = MatrixXf(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      _model(i, j) = matrix_data[i][j];
    }
  }
}

size_t Classifier::predict(const Features& features) const {
  size_t size = std::min(features.size(), static_cast<size_t>(_model.cols()));
  size_t rows = static_cast<size_t>(_model.rows());

  VectorXf vector(_model.cols());
  vector(0) = 1.0;
  for (size_t i = 1; i < size; ++i) {
    vector(i) = features[i];
  }

  VectorXf z_vector = _model * vector;
  VectorXf result_vector = 1.0 / (1.0 + (-z_vector.array()).exp());

  size_t result = 0;
  float max_result = 0;
  for (size_t i = 0; i < rows; ++i) {
    if (result_vector(i) > max_result) {
      result = i;
      max_result = result_vector(i);
    }
  }

  return result;
}
