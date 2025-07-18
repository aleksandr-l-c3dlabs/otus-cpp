#pragma once

#include <sstream>

bool get_column(std::istream& input, std::ostream& output,
                const u_int8_t column, const char splitter = ',');