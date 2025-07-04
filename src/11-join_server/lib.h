#pragma once

#include <sqlite3.h>
#include <functional>
#include <string>
#include <vector>

class Application {
  using Callback = std::function<void(const std::vector<std::string>&)>;

 public:
  Application();
  ~Application();

  std::string handle_command(const std::string& cmd);

 private:
  bool exec(
      const std::string& sql, std::string& error,
      Callback callback = [](const std::vector<std::string>) {});

 private:
  sqlite3* db;
};