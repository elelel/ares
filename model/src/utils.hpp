#pragma once

namespace ares {
  namespace model {
    /*! Copy raw bytes to a destination buffer of a predefined size and set unused destination bytes to 0.
      This is to avoid pessimistic initialization (zeroing out the whole dest buffer before copying
      \param dest destination buffer to copy the bytes to
      \param dest_sz destination buffer size
      \param src source buffer to copy the bytes from
      \param src_sz number of meaningful bytes in source buffer
    */
    inline static void copy_buf_with_zero_pad(void* dest, const size_t dest_sz, const void* src, const size_t src_sz);
  }
}
