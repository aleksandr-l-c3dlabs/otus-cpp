#include "server.h"

void Session::read_command() {
  auto self(shared_from_this());
  async_read_until(
      sock, buff, '\n',
      [self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          std::string cmd = buffer_cast<const char*>(self->buff.data());

          self->buff.consume(length);

          std::string response = self->app.handle_command(cmd);

          async_write(self->sock, boost::asio::buffer(response),
                      [self](boost::system::error_code ec, std::size_t) {
                        if (!ec) {
                          self->read_command();
                        }
                      });
        }
      });
}

void Server::do_accept() {
  acceptor.async_accept(
      [this](boost::system::error_code err, tcp::socket socket) {
        if (!err) {
          std::make_shared<Session>(std::move(socket))->start();
        }
        do_accept();
      });
}