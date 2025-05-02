#ifndef _UTF_8_HPP
#define _UTF_8_HPP
#include <cstdint>
#include <stdexcept>

// --- UTF-8 Decoder Implementation ---
char32_t decode_utf8(const char *&ptr, const char *end) {
  if (ptr >= end) {
    throw std::out_of_range("decode_utf8: Unexpected end of string");
  }

  unsigned char c1 = static_cast<unsigned char>(*ptr);
  uint32_t code_point = 0;

  if ((c1 & 0x80) == 0) { // 1-byte (ASCII)
    code_point = c1;
    ptr += 1;
  } else if ((c1 & 0xE0) == 0xC0) { // 2-byte
    if (ptr + 1 >= end)
      throw std::out_of_range("decode_utf8: Incomplete 2-byte sequence");
    unsigned char c2 = static_cast<unsigned char>(*(ptr + 1));
    if ((c2 & 0xC0) != 0x80)
      throw std::runtime_error("decode_utf8: Invalid 2-byte continuation");
    code_point = ((c1 & 0x1F) << 6) | (c2 & 0x3F);
    ptr += 2;
  } else if ((c1 & 0xF0) == 0xE0) { // 3-byte
    if (ptr + 2 >= end)
      throw std::out_of_range("decode_utf8: Incomplete 3-byte sequence");
    unsigned char c2 = static_cast<unsigned char>(*(ptr + 1));
    unsigned char c3 = static_cast<unsigned char>(*(ptr + 2));
    if ((c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80)
      throw std::runtime_error("decode_utf8: Invalid 3-byte continuation");
    code_point = ((c1 & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
    ptr += 3;
    if (code_point >= 0xD800 && code_point <= 0xDFFF) {
      throw std::runtime_error(
          "decode_utf8: Surrogate code points are invalid");
    }
  } else if ((c1 & 0xF8) == 0xF0) { // 4-byte
    if (ptr + 3 >= end)
      throw std::out_of_range("decode_utf8: Incomplete 4-byte sequence");
    unsigned char c2 = static_cast<unsigned char>(*(ptr + 1));
    unsigned char c3 = static_cast<unsigned char>(*(ptr + 2));
    unsigned char c4 = static_cast<unsigned char>(*(ptr + 3));
    if ((c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80 || (c4 & 0xC0) != 0x80)
      throw std::runtime_error("decode_utf8: Invalid 4-byte continuation");
    code_point = ((c1 & 0x07) << 18) | ((c2 & 0x3F) << 12) |
                 ((c3 & 0x3F) << 6) | (c4 & 0x3F);
    ptr += 4;
    if (code_point > 0x10FFFF) {
      throw std::runtime_error("decode_utf8: Code point out of Unicode range");
    }
  } else {
    throw std::runtime_error("decode_utf8: Invalid UTF-8 start byte");
  }
  return static_cast<char32_t>(code_point);
}
#endif
