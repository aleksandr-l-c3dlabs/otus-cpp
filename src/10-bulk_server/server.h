#pragma once

#include <boost/asio.hpp>
#include <memory>
#include "lib.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(tcp::socket socket, std::shared_ptr<Application> application)
      : sock(std::move(socket)), app(application) {}

  void start() { read_command(); }

 private:
  void read_command();

 private:
  tcp::socket sock;
  streambuf buff;
  std::shared_ptr<Application> app;
  void* context = nullptr;
};

class Server {
 public:
  Server(io_context& context, short port, std::shared_ptr<Application> app)
      : acceptor(context, tcp::endpoint(tcp::v4(), port)), application(app) {
    do_accept();
  }

  void do_accept();

 private:
  tcp::acceptor acceptor;
  std::shared_ptr<Application> application;
};