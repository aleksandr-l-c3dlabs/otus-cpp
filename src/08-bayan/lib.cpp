#include "lib.h"

#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <filesystem>
#include <fstream>

#include <iostream>

namespace fs = std::filesystem;

uint64_t crc32Hasher(const char* data, std::size_t size) {
  boost::crc_32_type crc;
  crc.process_bytes(data, size);
  return crc.checksum();
}

uint64_t md5Hasher(const char* data, std::size_t size) {
  boost::uuids::detail::md5 hash;
  boost::uuids::detail::md5::digest_type digest;

  hash.process_bytes(data, size);
  hash.get_digest(digest);

  // take only fist 64 bits (2 parts)
  return (static_cast<uint64_t>(digest[0]) << 32) | digest[1];
}

uint64_t sha1Hasher(const char* data, std::size_t size) {
  boost::uuids::detail::sha1 hash;
  boost::uuids::detail::sha1::digest_type digest;

  hash.process_bytes(data, size);
  hash.get_digest(digest);

  // take only fist 64 bits (2 parts)
  return (static_cast<uint64_t>(digest[0]) << 32) | digest[1];
}

EqualFinder::EqualFinder(HashAlg alg, std::size_t blockSize) : size(blockSize) {
  switch (alg) {
    case HashAlg::Crc32:
      hasher = crc32Hasher;
      break;

    case HashAlg::Md5:
      hasher = md5Hasher;
      break;

    case HashAlg::Sha1:
      hasher = sha1Hasher;
      break;
  }
}

std::vector<std::vector<std::string>> EqualFinder::getResult() const {
  std::vector<std::vector<std::string>> result{};
  for (const auto& group : groups) {
    if (group->files.size() > 1) {
      std::vector<std::string> filepaths;
      filepaths.reserve(group->files.size());
      for (const auto& info : group->files)
        filepaths.push_back(info.filepath);
      result.push_back(filepaths);
    }
  }
  return result;
}

uint64_t EqualFinder::getHash(const std::string& filepath,
                              std::size_t block) const {
  std::ifstream file(filepath, std::ios::binary);
  assert(file);

  std::vector<char> buffer(size, 0);
  file.seekg(block * size);
  file.read(buffer.data(), size);

  return hasher(buffer.data(), size);
}

FileInfo EqualFinder::getInfo(const std::string& filepath) const {
  return {filepath, static_cast<std::uintmax_t>(
                        std::ceil(fs::file_size(filepath) / size))};
}

void EqualFinder::put(const std::string& filepath) {
  if (!fs::exists(filepath))
    return;

  FileHash h;
  auto info = getInfo(filepath);
  bool not_found = true;

  do {
    WeakGroupList validated_groups;
    auto it = hashes.find(h.hash());
    if (it != hashes.end()) {
      validated_groups = it->second;
    }

    // filter groups with different file size
    validated_groups.erase(
        std::remove_if(validated_groups.begin(), validated_groups.end(),
                       [&info](std::weak_ptr<FileGroup> weak_group) {
                         bool result = true;
                         auto group = weak_group.lock();
                         if (group) {
                           result = group->blocks() != info.blocks;
                         }
                         return result;
                       }),
        validated_groups.end());

    if (validated_groups.empty()) {
      createGroup(info, h);
      break;
    }

    // read new block
    if (info.blocks > h.block_hashes.size())
      h.block_hashes.push_back(getHash(info.filepath, h.block_hashes.size()));

    // check groups with calculated the next block
    for (const auto& weak_group : validated_groups) {
      auto group = weak_group.lock();
      if (group && group->completed() && group->hash == h) {
        group->files.push_back(info);
        not_found = false;
        break;
      }
    }

    // check groups without calculated the next block
    if (not_found)
      for (auto weak_group : validated_groups) {
        auto group = weak_group.lock();
        while (group &&
               (group->hash.block_hashes.size() < h.block_hashes.size())) {
          readGroupBlock(group);

          // check block
          if (group->completed() && h == group->hash) {
            group->files.push_back(info);
            not_found = false;
            break;
          }
        }
      }

    // read the next block
  } while (not_found);
}

void EqualFinder::createGroup(const FileInfo& info, const FileHash& hash) {
  auto group = std::make_shared<FileGroup>();
  group->hash = hash;
  group->files.push_back(info);
  groups.push_back(group);
  FileHash parted;
  parted.block_hashes.reserve(hash.block_hashes.size());
  insertGroup(0, group);
  for (const auto& h : hash.block_hashes) {
    parted.block_hashes.push_back(h);
    insertGroup(parted.hash(), group);
  }
}

void EqualFinder::insertGroup(uint64_t key, std::weak_ptr<FileGroup> group) {
  auto it = hashes.find(key);
  if (it == hashes.end()) {
    hashes.insert({key, {group}});
  } else {
    it->second.push_back(group);
  }
}

void EqualFinder::readGroupBlock(std::shared_ptr<FileGroup> group) {
  if (group->completed() || group->files.size() != 1)
    return;

  // read group block
  group->hash.block_hashes.push_back(
      getHash(group->files.at(0).filepath, group->hash.block_hashes.size()));

  // push group
  insertGroup(group->hash.hash(), group);
}
