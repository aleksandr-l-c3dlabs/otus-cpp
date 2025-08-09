#include "primitives.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>

constexpr double epsilon = 1e-5;

bool Line::removePoint(const Position2D& pos) {
  bool result = false;
  _points.erase(std::remove_if(_points.begin(), _points.end(),
                               [&pos, &result](const Position3D& p) {
                                 bool v = pos.x == p.x && pos.y == p.y;
                                 result |= v;
                                 return v;
                               }),
                _points.end());
  return result;
}

bool Line::removePoint(const Position3D& pos) {
  bool result = false;
  _points.erase(std::remove_if(_points.begin(), _points.end(),
                               [&pos, &result](const Position3D& p) {
                                 bool v = pos.x == p.x && pos.y == p.y;
                                 result |= v;
                                 return v;
                               }),
                _points.end());
  return result;
}

void Line::draw() const {
  std::cout << "DRAW line: ";
  Position2D pos = position();
  for (const auto& p : _points) {
    std::cout << "(" << (pos + p) << ") ";
  }
  std::cout << std::endl;
}

bool Line::contain(int x, int y) const {
  bool result = false;
  std::optional<Position3D> prev;
  for (const auto& p2 : _points) {
    if (prev.has_value()) {
      auto p1 = prev.value();
      double cross_product =
          (p2.x - p1.x) * (y - p1.y) - (p2.y - p1.y) * (x - p1.x);
      result = std::abs(cross_product) < epsilon;
      if (result)
        break;
    }
    prev = p2;
  }
  return result;
}

void Rectangle::draw() const {
  std::cout << "DRAW rectangle: ";
  auto pos = position();
  auto w = static_cast<int>(width());
  auto h = static_cast<int>(height());
  std::vector<Position2D> points{pos, pos + Position2D{w, 0},
                                 pos + Position2D{w, h},
                                 pos + Position2D{-w, h}};
  for (const auto& p : points) {
    std::cout << "(" << p << ") ";
  }
  std::cout << std::endl;
}

bool Rectangle::contain(int x, int y) const {
  auto p = position();

  int minX = p.x;
  int maxX = p.x + width();
  int minY = p.y;
  int maxY = p.y + height();

  return (x >= minX && x <= maxX && y >= minY && y <= maxY);
}

void Circle::draw() const {
  std::cout << "DRAW circle: center (" << position() << ") radius " << radius()
            << std::endl;
}

bool Circle::contain(int x, int y) const {
  auto center = position();
  double distance_squared =
      std::pow(x - center.x, 2) + std::pow(y - center.y, 2);
  return distance_squared <= std::pow(radius(), 2);
}