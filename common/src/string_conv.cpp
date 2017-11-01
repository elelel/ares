#include "string_conv.hpp"

uint8_t ares::hexchar2halfbyte(const char c) {
  if (c >= '0' && c <= '9') 
    return c - '0';
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  return 0;
}

std::string ares::hex2string(const std::string& hex_string) {
  std::string rslt;
  if ((hex_string.size() > 0) && !(hex_string.size() % 2)) {
    const auto sz = hex_string.size() / 2;
    rslt.reserve(sz);
    for (size_t i = 0; i < sz; ++i) {
      rslt.push_back(char((hexchar2halfbyte(hex_string[i*2])) << 4) | hexchar2halfbyte(hex_string[i*2+1]));
    }
  }
  return rslt;
}

