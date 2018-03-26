#pragma once

#include <map>
#include <vector>
#include <optional>

/*!
  \file lattice.hpp

  Lattice (2d matrix) containers
 */

namespace ares {
  namespace model {
    namespace detail {
      struct lattice_base {
        inline lattice_base(const size_t x_size, const size_t y_size);
        inline size_t xy_to_index(const size_t x, const size_t y) const;
        inline std::tuple<size_t, size_t> index_to_xy(const size_t index) const;

      protected:
        size_t y_shift_;
        size_t x_mask_;
      };
    }

    /*! Dense lattice class.
      Access is constant, memory usage is full lattice space size
     */
    template <typename T>
    struct dense_lattice : detail::lattice_base {
      inline dense_lattice(const size_t x_size, const size_t y_size);
      
      inline size_t size() const;
      inline std::vector<T>& container();
      inline const std::vector<T>& container() const;
      inline T* data();
      inline const T* data() const;

      inline T& operator()(const size_t x, const size_t y);
      inline const T& operator()(const size_t x, const size_t y) const;
      inline T& operator[](const size_t i);
      inline const T& operator[](const size_t i) const;
    private:
      std::vector<T> data_;
    };

    /*! Sparse lattice class
      Access is O(log), memory usage is used lattice elements
     */
    template <typename T>
    struct sparse_lattice : detail::lattice_base {
      inline sparse_lattice(const size_t x_size, const size_t y_size);

      inline std::map<size_t, T>& container();
      inline const std::map<size_t, T>& container() const;
      inline size_t size() const;

      inline std::optional<T> operator()(const size_t x, const size_t y) const;
      inline std::optional<T> operator[](const size_t i) const;
      inline void insert(const size_t x, const size_t y, T&& v);
    private:
      std::map<size_t, T> data_;
    };
  }
}
