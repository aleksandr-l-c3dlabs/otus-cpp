#include "server.h"

void Session::read_command() {
  auto self(shared_from_this());
  async_read_until(
      sock, buff, '\n', [self](boost::system::error_code ec, std::size_t) {
        if (!ec) {
          std::istream is(&self->buff);
          std::string cmd;
          std::getline(is, cmd);

          if (!cmd.empty() && cmd.back() == '\r') {
            cmd.pop_back();
          }

          self->context = self->app->handle_command(cmd, self->context);

          self->read_command();
        }
      });
}

void Server::do_accept() {
  acceptor.async_accept(
      [this](boost::system::error_code err, tcp::socket socket) {
        if (!err) {
          std::make_shared<Session>(std::move(socket), application)->start();
        }
        do_accept();
      });
}