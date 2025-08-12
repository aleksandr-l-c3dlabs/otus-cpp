#pragma once

#include <eigen3/Eigen/Dense>
#include <vector>

using Eigen::MatrixXf;
using Eigen::VectorXf;

class Classifier {
 public:
  using Coefficients = std::vector<float>;
  using Features = std::vector<float>;

 public:
  Classifier(const std::vector<Coefficients>&);
  ~Classifier() = default;

  size_t predict(const Features& features) const;

 private:
  MatrixXf _model;
};