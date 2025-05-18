#include "lib.h"

#include <iostream>
#include <memory>

#include "logger.h"
#include "packer.h"
#include "reader.h"
#include "saver.h"

void Application::run(size_t pack_size) {
  auto reader = std::make_shared<Reader>();
  auto packer = std::make_shared<Packer>(pack_size);
  auto saver = std::make_shared<Saver>();
  auto logger = std::make_shared<Logger>();

  reader->addListener(packer);
  packer->addListener(logger);
  packer->addListener(saver);

  reader->run(std::cin);
}