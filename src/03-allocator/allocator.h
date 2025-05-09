#pragma once

#include <memory>
#include <vector>

struct deleter {
  void operator()(void* ptr) { ::operator delete(ptr); }
};

template <class T, std::size_t N>
struct stl_allocator {
  using value_type = T;

  stl_allocator() noexcept : used(new bool[N]()), capacity(N) {
    pools.push_back(
        std::shared_ptr<void>(::operator new(sizeof(T) * N), deleter()));
  }

  template <class U, std::size_t UN>
  stl_allocator(const stl_allocator<U, UN>& a) {
    pools = a.pools;
    used = a.used;
    capacity = a.capacity;
  }

  ~stl_allocator() { delete[] used; }

  T* allocate(std::size_t n) {
    if (n > N)
      throw std::bad_alloc();

    bool found = false;
    std::size_t p = 0;
    for (std::size_t i = 0; i < capacity; ++i) {
      if (used[i] == false)
        for (std::size_t j = 0; j < n; ++j) {
          if (i + j >= capacity || used[i + j] == true)
            break;
          if (j + 1 == n) {
            p = i;
            found = true;
          }
        }
      if (found)
        break;
    }

    if (!found) {
      std::shared_ptr<void> new_pool(::operator new(sizeof(T) * N), deleter());
      auto new_used = new bool[capacity + N];

      std::copy(used, used + capacity * sizeof(bool), new_used);

      p = capacity;
      capacity += N;
      pools.push_back(new_pool);
      used = new_used;
    }

    for (std::size_t i = 0; i < n; ++i)
      used[p + i] = true;

    auto pool = pools[p / N];
    return static_cast<T*>(pool.get()) + (p % N);
  }

  void deallocate(T* p, std::size_t n) {
    auto pool = pools.front();
    for (const auto& _pool : pools) {
      std::size_t diff = p - static_cast<T*>(_pool.get());
      if (diff < N) {
        pool = _pool;
        break;
      }
    }
    auto i = p - static_cast<T*>(pool.get());
    while (n > 0)
      used[i + (--n)] = false;
  }

  std::size_t max_size() const noexcept { return N; }

  template <class U>
  struct rebind {
    typedef stl_allocator<U, N> other;
  };

  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap =
      std::true_type;  // UB if std::false_type and a1 != a2;

 private:
  std::vector<std::shared_ptr<void>> pools;
  bool* used;
  std::size_t capacity;
};

template <class T, std::size_t N, class U, std::size_t M>
constexpr bool operator==(const stl_allocator<T, N>& a1,
                          const stl_allocator<U, M>& a2) {
  return N == M && a1.pool == a2.pool;
}

template <class T, std::size_t N, class U, std::size_t M>
constexpr bool operator!=(const stl_allocator<T, N>& a1,
                          const stl_allocator<U, M>& a2) {
  return N != M || a1.pool != a2.pool;
}
