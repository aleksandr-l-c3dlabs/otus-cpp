#pragma once

#include <vector>

#include "models/coords.h"
#include "models/primitives.h"

/// @brief User viewport
class Viewport {
 public:
  /// @brief Default construct
  Viewport() {}

  /// @brief Constructor with specified size
  /// @param width Viewport width
  /// @param height Viewport height
  Viewport(int width, int height) : _width(width), _height(height) {}

  /// @brief Update viewport size
  /// @param width New viewport width
  /// @param height New viewport height
  void resize(int width, int height);

  /// @brief Add primitives on viewport
  /// @param[in] primitives A primitive list
  void draw(const std::vector<Primitive::ConstWeakPtr>& primitives);

  /// @brief Clear viewport
  void clear() {}

 private:
  int _width = 0;
  int _height = 0;
};
