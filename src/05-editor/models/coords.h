#pragma once

#include <ostream>

struct Position3D;

/**
 * @brief Coordinate position at 2D area
 */
struct Position2D {
  /**
     * @brief X coordinate
     */
  int x;

  /**
     * @brief Y coordinate
     */
  int y;

  /**
     * @brief Sum of two positions
     */
  Position2D& operator+=(const Position2D& pos) {
    x += pos.x;
    y += pos.y;
    return *this;
  }

  /**
     * @brief Sum of two positions
     */
  Position2D operator+(const Position2D& pos) const {
    return Position2D{x + pos.x, y + pos.y};
  }

  /**
     * @brief Sum of two positions
     */
  Position2D operator+(const Position3D& pos);
};

/**
 * @brief Coordinate position at 3D area
 */
struct Position3D {
  /**
     * @brief X coordinate
     */
  int x;

  /**
     * @brief Y coordinate
     */
  int y;

  /**
     * @brief Z coordinates
     * @details Required for resolving layer conflicts
     */
  int z;

  /**
     * @brief Get 2D position
     * @details Ignore layer
     */
  Position2D xy() const { return {x, y}; }

  /**
     * @brief Apply 2D position as a part of 3D position
     * @param[in] pos 2D position
     */
  Position3D& operator=(const Position2D& pos) {
    x = pos.x;
    y = pos.y;
    return *this;
  }

  /**
     * @brief Sum of two positions
     */
  Position3D& operator+=(const Position3D& pos) {
    x += pos.x;
    y += pos.y;
    z += pos.z;
    return *this;
  }

  /**
     * @brief Sum of two positions
     */
  Position3D operator+(const Position3D& pos) const {
    return Position3D{x + pos.x, y + pos.y, z + pos.z};
  }

  /**
     * @brief Sum of 3D position with 2D position
     */
  Position3D operator+(const Position2D& pos) const {
    return Position3D{x + pos.x, y + pos.y, z};
  }
};

inline std::ostream& operator<<(std::ostream& os, const Position2D& pos) {
  os << pos.x << "," << pos.y << std::endl;
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const Position3D& pos) {
  os << pos.x << "," << pos.y << std::endl;
  return os;
}

inline Position2D operator+(const Position2D& pos1, const Position3D& pos2) {
  return Position2D{pos1.x + pos2.x, pos1.y + pos2.y};
}
