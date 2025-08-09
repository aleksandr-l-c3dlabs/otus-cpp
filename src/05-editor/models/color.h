#pragma once

#include <cstdint>

/**
 * @brief Color class
 */
class Color {
 public:
  /**
     * @brief Default color constructor (black color)
     */
  Color() : data(0) {}

  /**
     * @brief RGBA color constructor
     * @details Every color component is a byte value and color is 4-bite value
     * @param[in] rgba A single parameter of all color components
     */
  Color(uint32_t rgba) : data(rgba) {}

  /**
     * @brief RGB color constructor
     * @details Every color component is a byte value. Alpha component is a maximum.
     * @param[in] r Red color component
     * @param[in] g Green color component
     * @param[in] b Blue color component
     */
  Color(uint8_t r, uint8_t g, uint8_t b) : data(0) {
    data |= static_cast<uint32_t>(r) << 24;
    data |= static_cast<uint32_t>(g) << 16;
    data |= static_cast<uint32_t>(b) << 8;
    data |= static_cast<uint32_t>(255);
  }

 private:
  uint32_t data;
};