inline static void ares::model::copy_buf_with_zero_pad(void* dest, const size_t dest_sz, const void* src, const size_t src_sz) {
  const auto len = (src_sz > dest_sz) ? dest_sz : src_sz;
  memcpy(dest, src, len);
  memset((void*)((uintptr_t)dest + len), 0, dest_sz - len);
}
