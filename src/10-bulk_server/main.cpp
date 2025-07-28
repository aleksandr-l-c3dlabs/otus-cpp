#include <boost/asio.hpp>
#include <iostream>
#include "lib.h"
#include "server.h"

using namespace boost::asio::ip;

int main(const int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Required paramater (port)" << std::endl;
    return 1;
  } else if (argc < 3) {
    std::cerr << "Error: Required parameter (bulk_size)" << std::endl;
    return 2;
  }

  try {
    auto app = std::make_shared<Application>(atoi(argv[2]));
    io_context io;
    Server server(io, atoi(argv[1]), app);
    io.run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 3;
  }

  return 0;
}