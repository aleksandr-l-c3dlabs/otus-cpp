#pragma once

#include <stdint.h>
#include <boost/functional/hash.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

enum class HashAlg : uint8_t {
  Crc32,
  Md5,
  Sha1,
};

struct FileInfo {
  std::string filepath;
  std::uintmax_t blocks;
};

struct FileHash {
  std::vector<uint64_t> block_hashes;

  std::size_t hash() const {
    std::size_t seed = 0;
    for (const auto& hash : block_hashes)
      boost::hash_combine(seed, hash);
    return seed;
  }

  bool operator==(const FileHash& other) const {
    if (block_hashes.size() != other.block_hashes.size())
      return false;

    bool result = true;
    for (std::size_t i = 0; i < block_hashes.size(); i++) {
      result &= block_hashes.at(i) == other.block_hashes.at(i);
      if (!result)
        break;
    }
    return result;
  }
};

struct FileGroup {
  FileHash hash;
  std::vector<FileInfo> files;

  std::size_t blocks() {
    // all files have this block count
    return files.empty() ? 0 : files.at(0).blocks;
  }

  bool completed() {
    // all block read
    return !files.empty() && hash.block_hashes.size() == blocks();
  }
};

// struct HashWrapper {

//     bool operator==(const HashWrapper& other) const {
//         if (block_hashes.size() != other.block_hashes.size()) return false;

//         bool result = true;
//         for (std::size_t i = 0; i < block_hashes.size(); i++) {
//             result &= block_hashes.at(i) == other.block_hashes.at(i);
//             if (!result) break;
//         }
//         return result;
//     }
// };

// struct HashWrapperHasher {
//     std::size_t operator()(const HashWrapper& h) const {
//         std::size_t seed = 0;
//         for (const auto& hash : h.block_hashes)
//             boost::hash_combine(seed, hash);
//         return seed;
//     }
// };

class EqualFinder {
 private:
  using GroupList = std::vector<std::shared_ptr<FileGroup>>;
  using WeakGroupList = std::vector<std::weak_ptr<FileGroup>>;

 public:
  EqualFinder(HashAlg alg, std::size_t blockSize);
  void put(const std::string& filepath);
  std::vector<std::vector<std::string>> getResult() const;

 private:
  uint64_t getHash(const std::string& filepath, std::size_t block) const;
  FileInfo getInfo(const std::string& filepath) const;
  void createGroup(const FileInfo& info, const FileHash& hash = {});
  void insertGroup(uint64_t key, std::weak_ptr<FileGroup> group);
  void readGroupBlock(std::shared_ptr<FileGroup> group);
  // void appendToGroup(const FileInfo& info, std::weak_ptr<FileGroup> group);
  // std::weak_ptr<FileGroup> createGroup(const FileHash& hash = {});
  // void appendToGroup(const FileInfo& info, const FileHash& hash, std::weak_ptr<FileGroup> group);
 private:
  std::size_t size;
  std::function<uint64_t(const char* data, std::size_t size)> hasher;
  GroupList groups;
  std::unordered_map<std::size_t, WeakGroupList> hashes;
};