
#include <boost/asio.hpp>
#include <iostream>
#include "server.h"

using namespace boost::asio::ip;

int main(const int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Required paramater (port)" << std::endl;
    return 1;
  }

  try {
    io_context io;
    Server server(io, atoi(argv[1]));
    io.run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }

  return 0;
}