#include "reader.h"
#include <csignal>
#include <functional>

std::atomic<bool> Reader::interrupted = false;
std::istream* Reader::in = nullptr;

Reader::Reader() {
  std::signal(SIGINT, signalHandler);
}

Reader::~Reader() {
  std::signal(SIGINT, SIG_DFL);
  in = nullptr;
}

void Reader::signalHandler(int code) {
  if (code == SIGINT) {
    interrupted = true;
    if (in) {
      in->setstate(std::ios::eofbit);
    }
  }
}

void Reader::run(std::istream& input) {
  in = &input;

  std::string cmd;
  while (!interrupted && std::getline(input, cmd)) {
    notifyListeners(InputCommandEvent{cmd, false});
  }
  notifyListeners(InputCommandEvent{"", true});
}