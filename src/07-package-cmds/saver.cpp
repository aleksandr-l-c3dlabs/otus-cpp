#include "saver.h"

#include <fstream>

Saver::Saver() {}

void Saver::onEvent(const PackEvent& event) {
  std::string filename =
      std::string{"bulk"} + std::to_string(event.created) + ".log";
  std::ofstream fp(filename);
  for (const auto& cmd : event.commands) {
    fp << cmd << std::endl;
  }
  fp.close();
}