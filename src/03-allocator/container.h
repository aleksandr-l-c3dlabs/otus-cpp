#pragma once

#include <algorithm>
#include <memory>

template <class T, class Allocator = std::allocator<T>>
struct stl_container {
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = pointer;
  using const_iterator = const_pointer;

  stl_container() = default;

  ~stl_container() {
    if (m_data) {
      for (size_t i = 0; i < m_size; ++i) {
        std::allocator_traits<Allocator>::destroy(alloc, &m_data[i]);
      }
      std::allocator_traits<Allocator>::deallocate(alloc, m_data, capacity);
    }
  }

  void push_back(const T& x) {
    if (m_size == capacity) {
      reserve(
          capacity == 0
              ? 1
              : std::min(capacity * 2,
                         std::allocator_traits<Allocator>::max_size(alloc)));
    }

    std::allocator_traits<Allocator>::construct(alloc, &m_data[m_size], x);
    ++m_size;
  }

  void reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity)
      return;

    T* newData =
        std::allocator_traits<Allocator>::allocate(alloc, new_capacity);

    for (size_t i = 0; i < m_size; ++i) {
      std::allocator_traits<Allocator>::construct(
          alloc, &newData[i], std::move_if_noexcept(m_data[i]));
      std::allocator_traits<Allocator>::destroy(alloc, &m_data[i]);
    }

    if (m_data) {
      std::allocator_traits<Allocator>::deallocate(alloc, m_data, capacity);
    }

    m_data = newData;
    capacity = new_capacity;
  }

  iterator begin() noexcept { return m_data; }
  const_iterator begin() const noexcept { return m_data; }
  iterator end() noexcept { return begin() + m_size; }
  const_iterator end() const noexcept { return begin() + m_size; }

  value_type* data() noexcept { return m_data; }
  const value_type* data() const noexcept { m_data; }
  size_type size() const noexcept { return m_size; }
  bool empty() const noexcept { return m_size > 0; }

 private:
  std::size_t m_size = 0;
  std::size_t capacity = 0;
  T* m_data = nullptr;

  Allocator alloc;
};
