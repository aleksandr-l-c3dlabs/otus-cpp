#include "async.h"

#include <packer.h>
#include <chrono>
#include <memory>
#include "receiver.h"
#include "thread-listener.h"

static auto global = std::make_shared<ThreadListener>();

struct Context {
  std::shared_ptr<Receiver> receiver;
  std::shared_ptr<Packer> packer;
};

void* connect(std::size_t size) {
  auto receiver = std::make_shared<Receiver>();
  auto packer = std::make_shared<Packer>(size);
  auto ctx = new Context{receiver, packer};
  receiver->addListener(packer);
  packer->addListener(global);
  return ctx;
}

void receive(const char* ref, std::size_t length, void* ctx) {
  auto context = reinterpret_cast<Context*>(ctx);
  if (context) {
    context->receiver->receive(std::string(ref, length));
  }
}

void disconnect(void* ctx) {
  auto context = reinterpret_cast<Context*>(ctx);
  if (context) {
    context->receiver.reset();
    // Wait for last command pack complete done
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    delete (context);
  }
}