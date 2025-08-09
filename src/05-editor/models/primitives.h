#pragma once

#include <memory>
#include <string>
#include <vector>

#include "color.h"
#include "coords.h"

using PX = uint32_t;

/**
 * @brief Supported SVG types
 */
enum class PrimitiveType {
  Line,
  Rectangle,
  Circle,
};

/**
 * @brief Base SVG primitive class
 */
class Primitive {
 public:
  using Ptr = std::shared_ptr<Primitive>;
  using WeakPtr = std::weak_ptr<Primitive>;
  using ConstWeakPtr = std::weak_ptr<const Primitive>;

 public:
  /// @brief Base constructor
  Primitive() : _position({0, 0, 0}) {}
  /// @brief Base destructor
  ~Primitive() = default;

  /// @brief Change primitive position
  /// @param[in] pos New 2D position
  void move(const Position2D& pos) { _position = pos; }

  /// @brief Change primitive position
  /// @param[in] pos New 3D position
  void move(const Position3D& pos) { _position = pos; }

  /// @brief Get current primitive position
  /// @return Current 2D position
  Position2D position() const { return _position.xy(); }

  /// @brief Get current layer
  /// @return Current layer position
  int layer() const { return _position.z; }

  /// @brief Set the new fill color
  /// @param[in] color New fill color
  void fill(const Color& color) { _fill_color = color; }

  /// @brief Get current fill color
  /// @return Current fill color
  const Color& fill() const { return _fill_color; }

  /// @brief Set the new stroke color
  /// @param[in] color New stroke color
  void stroke(const Color& color) { _stroke_color = color; }

  /// @brief Get current stroke color
  /// @return Current stroke color
  const Color& stroke() const { return _stroke_color; }

  /// @brief Redraw the primitive
  virtual void draw() const = 0;

  /// @brief Check if the specified point belongs to the given primitive
  /// @param[in] x Point coordinate X
  /// @param[in] y Point coordinate Y
  /// @return True if the point in primitive else false
  virtual bool contain(int x, int y) const = 0;

  /// @brief Get current primitive type
  /// @return Primitive type
  virtual PrimitiveType type() const = 0;

 private:
  Position3D _position;
  Color _fill_color;
  Color _stroke_color;
};

/// @brief Multipoint line primitive
class Line : public Primitive {
 public:
  /// @brief Add a new point to line
  /// @param[in] pos New point position
  void addPoint(const Position2D& pos) {
    _points.push_back(Position3D{pos.x, pos.y, 0});
  }

  /// @brief Add a new point to line
  /// @param[in] pos New point position
  void addPoint(const Position3D& pos) { _points.push_back(pos); }

  /// @brief Remove a point from line with layer ignoring
  /// @param[in] pos Removed point position
  bool removePoint(const Position2D& pos);

  /// @brief Remove a point from line
  /// @param[in] pos Removed point position
  bool removePoint(const Position3D& pos);

  /// @brief Render line
  void draw() const override;

  /// @brief Check if the specified point belongs to the given primitive
  /// @param[in] x Point coordinate X
  /// @param[in] y Point coordinate Y
  /// @return True if the point in primitive else false
  bool contain(int x, int y) const override;

  /// @brief Get current primitive type
  /// @return Primitive type
  PrimitiveType type() const override { return PrimitiveType::Line; }

 private:
  /// @brief A list of points
  std::vector<Position3D> _points;
};

/// @brief Rectangle primitive (position + size)
class Rectangle : public Primitive {
 public:
  /// @brief Set the new rectangle width
  /// @param[in] value Width
  void width(PX value) { _width = value; }

  /// @brief Get the current rectangle width
  /// @return Width
  PX width() const { return _width; }

  /// @brief Set the new rectangle height
  /// @param[in] value Height
  void height(PX value) { _height = value; }

  /// @brief Get the current rectangle height
  /// @return Height
  PX height() const { return _height; }

  /// @brief Render line
  void draw() const override;

  /// @brief Check if the specified point belongs to the given primitive
  /// @param[in] x Point coordinate X
  /// @param[in] y Point coordinate Y
  /// @return True if the point in primitive else false
  bool contain(int x, int y) const override;

  /// @brief Get current primitive type
  /// @return Primitive type
  PrimitiveType type() const override { return PrimitiveType::Rectangle; }

 private:
  PX _width, _height;
};

/// @brief Circle primitive (position + radius)
class Circle : public Primitive {
 public:
  /// @brief Set the new circle radius
  /// @param[in] value Circle radius
  void radius(PX value) { _radius = value; }

  /// @brief Get the current circle radius
  /// @return Circle radius
  PX radius() const { return _radius; }

  /// @brief Render line
  void draw() const override;

  /// @brief Check if the specified point belongs to the given primitive
  /// @param[in] x Point coordinate X
  /// @param[in] y Point coordinate Y
  /// @return True if the point in primitive else false
  bool contain(int x, int y) const override;

  /// @brief Get current primitive type
  /// @return Primitive type
  PrimitiveType type() const override { return PrimitiveType::Circle; }

 private:
  PX _radius;
};
