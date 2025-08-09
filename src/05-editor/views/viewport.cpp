#include "viewport.h"

void Viewport::resize(int width, int height) {
  _width = width;
  _height = height;
}

void Viewport::draw(const std::vector<Primitive::ConstWeakPtr>& primitives) {
  for (const auto& pr : primitives) {
    auto ptr = pr.lock();
    if (!ptr)
      continue;
    ptr->draw();
  }
}