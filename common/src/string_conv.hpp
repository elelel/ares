#pragma once

#include <string>

namespace ares {
  uint8_t hexchar2halfbyte(const char c);
  std::string hex2string(const std::string& hex_string);
}
              
