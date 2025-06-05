#pragma once

#include <iostream>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <unordered_map>

template <typename T, T default_value, std::size_t N>
class Matrix;

template <typename T, T default_value>
class ProxyCell {
 private:
  using DataMap = std::unordered_map<std::size_t, T>;

  DataMap& current_map;
  std::size_t index;

 public:
  ProxyCell(std::size_t idx, DataMap& map) : current_map(map), index(idx) {}

  // get value
  operator T() const {
    auto it = current_map.find(index);
    return (it != current_map.end()) ? it->second : default_value;
  }

  // set value
  ProxyCell& operator=(const T& value) {
    if (value == default_value) {
      current_map.erase(index);
    } else {
      current_map[index] = value;
    }
    return *this;
  }
};

template <typename T, T default_value, std::size_t N>
class Proxy {
 private:
  using DataMap = std::unordered_map<std::size_t, T>;
  using NestedMap =
      std::unordered_map<std::size_t, Matrix<T, default_value, N>>;
  using ReturnType = std::conditional_t<N == 1, ProxyCell<T, default_value>,
                                        Proxy<T, default_value, N - 1>>;

  std::conditional_t<N == 0, DataMap&, NestedMap&> current_map;
  std::size_t index;

 public:
  Proxy(std::size_t idx, decltype(current_map) map)
      : current_map(map), index(idx) {}

  // get inner proxy
  ReturnType operator[](std::size_t idx) {
    if constexpr (N == 0)
      return current_map[index];
    return current_map[index][idx];
  }
};

template <typename T, T default_value, std::size_t N = 2>
class Matrix {
 private:
  using DataMap = std::unordered_map<std::size_t, T>;
  using NestedMap =
      std::unordered_map<std::size_t, Matrix<T, default_value, N - 1>>;
  NestedMap data;

 public:
  Proxy<T, default_value, N - 1> operator[](std::size_t idx) {
    return Proxy<T, default_value, N - 1>(idx, data);
  }

  std::size_t size() const {
    std::size_t count = 0;
    for (const auto& pair : data) {
      count += pair.second.size();
    }
    return count;
  }

  class Iterator {
   private:
    using MapIterator = typename NestedMap::const_iterator;
    MapIterator it;
    MapIterator end_it;
    typename Matrix<T, default_value, N - 1>::Iterator nested_it;

   public:
    Iterator(MapIterator begin, MapIterator end)
        : it(begin), end_it(end), nested_it(it->second.end()) {
      if (it != end_it) {
        nested_it = it->second.begin();
        advance_to_valid();
      }
    }

    void advance_to_valid() {
      while (true) {
        if (it == end_it)
          break;
        if (nested_it == it->second.end()) {
          if (++it != end_it) {
            nested_it = it->second.begin();
          }
          continue;
        }
        break;
      }
    }

    Iterator& operator++() {
      if (++nested_it == it->second.end()) {
        if (++it != end_it) {
          nested_it = it->second.begin();
        }
      }
      advance_to_valid();
      return *this;
    }

    auto operator*() const {
      return std::tuple_cat(std::make_tuple(it->first), *nested_it);
    }

    bool operator==(const Iterator& other) const {
      return it == other.it && nested_it == other.nested_it;
    }
    bool operator!=(const Iterator& other) const { return !(*this == other); }
  };

  Iterator begin() const { return Iterator(data.begin(), data.end()); }
  Iterator end() const { return Iterator(data.end(), data.end()); }
};

template <typename T, T default_value>
class Matrix<T, default_value, 1> {
 private:
  using DataMap = std::unordered_map<std::size_t, T>;
  DataMap data;

 public:
  ProxyCell<T, default_value> operator[](std::size_t idx) {
    return ProxyCell<T, default_value>(idx, data);
  }

  T operator[](std::size_t idx) const {
    auto it = data.find(idx);
    return (it != data.end()) ? it->second : default_value;
  }

  std::size_t size() const { return data.size(); }

  class Iterator {
   private:
    using MapIterator = typename DataMap::const_iterator;
    MapIterator it;
    MapIterator end_it;

   public:
    Iterator(MapIterator begin, MapIterator end) : it(begin), end_it(end) {}

    Iterator& operator++() {
      ++it;
      return *this;
    }

    auto operator*() const { return std::make_tuple(it->first, it->second); }

    bool operator==(const Iterator& other) const { return it == other.it; }
    bool operator!=(const Iterator& other) const { return !(*this == other); }
  };

  Iterator begin() const { return Iterator(data.begin(), data.end()); }
  Iterator end() const { return Iterator(data.end(), data.end()); }
};