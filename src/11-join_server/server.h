#pragma once

#include <boost/asio.hpp>
#include <memory>
#include "lib.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(tcp::socket socket) : sock(std::move(socket)) {}

  void start() { read_command(); }

 private:
  void read_command();

 private:
  tcp::socket sock;
  streambuf buff;
  Application app;
};

class Server {
 public:
  Server(io_context& context, short port)
      : acceptor(context, tcp::endpoint(tcp::v4(), port)) {
    do_accept();
  }

  void do_accept();

 private:
  tcp::acceptor acceptor;
};