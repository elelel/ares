#include "../lattice.hpp"

namespace ares {
  namespace model {
    namespace detail {
      constexpr size_t count_significant_bits(const size_t n) {
        size_t i{0};
        for (; i < sizeof(n) * 8; ++i) {
          if ((n >> i) == 0) break;
        }
        return i;
      }
    }
  }
}

inline ares::model::detail::lattice_base::lattice_base(const size_t x_size, const size_t) :
  y_shift_(detail::count_significant_bits(x_size - 1)),
  x_mask_(0) {
  for (size_t i = 0; i < y_shift_; ++i) x_mask_ |= size_t(1) << i;
}  

inline size_t ares::model::detail::lattice_base::xy_to_index(const size_t x, const size_t y) const {
  return x | (y << y_shift_);
}

inline std::tuple<size_t, size_t> ares::model::detail::lattice_base::index_to_xy(const size_t i) const {
  return std::tuple<size_t, size_t>(i & x_mask_, i >> y_shift_);
}

template <typename T>
inline ares::model::dense_lattice<T>::dense_lattice(const size_t x_size, const size_t y_size) :
  detail::lattice_base(x_size, y_size),
  data_(size_t(1) << detail::count_significant_bits((y_size - 1) << y_shift_)) {
}

template <typename T>
inline size_t ares::model::dense_lattice<T>::size() const {
  return data_.size();
}

template <typename T>
inline T* ares::model::dense_lattice<T>::data() {
  return data_.data();
}

template <typename T>
inline const T* ares::model::dense_lattice<T>::data() const {
  return data_.data();
}

template <typename T>
inline std::vector<T>& ares::model::dense_lattice<T>::container() {
  return data_;
}

template <typename T>
inline const std::vector<T>& ares::model::dense_lattice<T>::container() const {
  return data_;
}

template <typename T>
inline T& ares::model::dense_lattice<T>::operator()(const size_t x, const size_t y) {
  return data_[xy_to_index(x, y)];
}

template <typename T>
inline const T& ares::model::dense_lattice<T>::operator()(const size_t x, const size_t y) const {
  return data_[xy_to_index(x, y)];
}

template <typename T>
inline T& ares::model::dense_lattice<T>::operator[](const size_t i) {
  return data_[i];
}

template <typename T>
inline const T& ares::model::dense_lattice<T>::operator[](const size_t i) const {
  return data_[i];
}

template <typename T>
inline ares::model::sparse_lattice<T>::sparse_lattice(const size_t x_size, const size_t y_size) :
  detail::lattice_base(x_size, y_size) {
}

template <typename T>
inline size_t ares::model::sparse_lattice<T>::size() const {
  return data_.size();
}

template <typename T>
inline std::map<size_t, T>& ares::model::sparse_lattice<T>::container() {
  return data_;
}

template <typename T>
inline const std::map<size_t, T>& ares::model::sparse_lattice<T>::container() const {
  return data_;
}

template <typename T>
inline std::optional<T> ares::model::sparse_lattice<T>::operator()(const size_t x, const size_t y) const {
  auto found = data_.find(xy_to_index(x, y));
  if (found != data_.end()) {
    return std::optional<T>(found->second);
  }
  return std::optional<T>();
}

template <typename T>
inline std::optional<T> ares::model::sparse_lattice<T>::operator[](const size_t i) const {
  auto found = data_.find(i);
  if (found != data_.end()) {
    return std::optional<T>(found->second);
  }
  return std::optional<T>();
}

template <typename T>
inline void ares::model::sparse_lattice<T>::insert(const size_t x, const size_t y, T&& v) {
  data_.insert(xy_to_index(x, y), std::forward<T>(v));
}
