#include <gtest/gtest.h>

#include <lib.h>
#include <openssl/evp.h>
#include <fstream>
#include "roots.h"

std::string calculateMD5(const std::string& data) {
  EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    throw std::runtime_error("Failed to create EVP_MD_CTX");
  }

  if (EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr) != 1) {
    EVP_MD_CTX_free(mdctx);
    throw std::runtime_error("Failed to initialize MD5 digest");
  }

  if (EVP_DigestUpdate(mdctx, data.c_str(), data.size()) != 1) {
    EVP_MD_CTX_free(mdctx);
    throw std::runtime_error("Failed to update MD5 digest");
  }

  unsigned char result[EVP_MAX_MD_SIZE];
  unsigned int resultLen;
  if (EVP_DigestFinal_ex(mdctx, result, &resultLen) != 1) {
    EVP_MD_CTX_free(mdctx);
    throw std::runtime_error("Failed to finalize MD5 digest");
  }

  EVP_MD_CTX_free(mdctx);

  std::ostringstream hexStream;
  for (unsigned int i = 0; i < resultLen; ++i) {
    hexStream << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(result[i]);
  }

  return hexStream.str();
}

TEST(lib, result_hashsum) {
  const std::string filePath = std::string(TEST_DATA_ROOT) + "ip_filter.tsv";
  std::ifstream file(filePath, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Could not open file: " + filePath);
  }

  std::ostringstream outputStream;
  ip_filter(file, outputStream);

  auto hashsum = calculateMD5(outputStream.str());

  EXPECT_EQ(hashsum, "24e7a7b2270daee89c64d3ca5fb3da1a");
}
