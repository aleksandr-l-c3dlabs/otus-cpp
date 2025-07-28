#include "lib.h"

#include <shared/async.h>
#include <atomic>
#include <csignal>
#include <iostream>
#include <string>

std::atomic<bool> interrupted = false;

void signalHandler(int code) {
  if (code == SIGINT) {
    interrupted = true;
    std::cin.setstate(std::ios::eofbit);
  }
}

AsyncDemo::AsyncDemo() {
  std::signal(SIGINT, signalHandler);
}

AsyncDemo::~AsyncDemo() {
  std::signal(SIGINT, SIG_DFL);
}

void AsyncDemo::run(std::size_t pack_size) {
  auto ctx = connect(pack_size);

  std::string cmd;
  while (!interrupted && std::getline(std::cin, cmd)) {
    receive(cmd.c_str(), cmd.size(), ctx);
  }

  disconnect(ctx);
}