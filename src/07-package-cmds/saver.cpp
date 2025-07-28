#include "saver.h"

#include <algorithm>
#include <fstream>
#include <random>
#include <string>

std::string generateRandomString(size_t length) {
  const std::string characters =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  std::random_device rd;
  std::mt19937 generator(rd());

  std::uniform_int_distribution<> distribution(0, characters.length() - 1);

  std::string randomString;
  randomString.reserve(length);

  for (size_t i = 0; i < length; ++i) {
    randomString += characters[distribution(generator)];
  }

  return randomString;
}

Saver::Saver(bool randPf) : randomPostfix(randPf) {}

void Saver::onEvent(const PackEvent& event) {
  std::string pf = "";
  if (randomPostfix) {
    pf += "_";
    pf += generateRandomString(5);
  }

  std::string filename =
      std::string{"bulk"} + std::to_string(event.created) + pf + ".log";

  std::ofstream fp(filename);
  for (const auto& cmd : event.commands) {
    fp << cmd << std::endl;
  }
  fp.close();
}