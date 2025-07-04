#include "lib.h"

#include <boost/algorithm/string/join.hpp>
#include <sstream>

Application::Application() {
  sqlite3_open(":memory:", &db);

  std::string err;
  exec(
      "CREATE TABLE A(id INT, name TEXT);"
      "CREATE TABLE B(id INT, name TEXT);"
      // "INSERT INTO A VALUES (0,'lean'), (1,'sweater'), (2,'frank'), "
      // "(3,'violation'), (4,'quality'), (5,'precision');"
      // "INSERT INTO B VALUES (6,'flour'), (4,'example'), (7,'wonder'), "
      // "(8,'selection'), (3,'proposal'), (5,'lake');"
      ,
      err);
}

Application::~Application() {
  sqlite3_close(db);
}

std::string Application::handle_command(const std::string& cmd) {
  std::string response;
  std::istringstream iss(cmd);
  std::string action, table, name, error;
  int id;

  iss >> action;

  if (action == "INSERT") {
    // INSERT table id name
    iss >> table >> id >> name;

    exec("INSERT INTO " + table + " VALUES(" + std::to_string(id) + ", '" +
             name + "');",
         error);
  } else if (action == "TRUNCATE") {
    // TRUNCATE table
    iss >> table;

    exec("DELETE FROM " + table + ";", error);
  } else if (action == "INTERSECTION") {
    exec(
        "SELECT A.id, A.name, B.name FROM A JOIN B ON A.id = B.id ORDER BY "
        "A.id;",
        error, [&response](const std::vector<std::string> columns) {
          response += boost::algorithm::join(columns, ",") + '\n';
        });
  } else if (action == "SYMMETRIC_DIFFERENCE") {
    exec(
        "SELECT A.id AS key, A.name, B.name "
        "FROM A LEFT JOIN B ON A.id = B.id "
        "WHERE B.id IS NULL "
        "UNION ALL "
        "SELECT B.id AS key, A.name, B.name "
        "FROM B LEFT JOIN A ON A.id = B.id "
        "WHERE A.id IS NULL "
        "ORDER BY key;",
        error, [&response](const std::vector<std::string>& columns) {
          response += boost::algorithm::join(columns, ",") + '\n';
        });
  } else {
    error = "Invalid command";
  }

  response += error.empty() ? "OK\n" : "ERR: " + error + '\n';
  return response;
}

bool Application::exec(const std::string& sql, std::string& error,
                       Callback callback) {
  char* err;
  auto res = sqlite3_exec(
      db, sql.c_str(),
      [](void* context, int argc, char** argv, char**) {
        auto& callback = *static_cast<Callback*>(context);
        std::vector<std::string> result;
        result.reserve(argc);
        for (int i = 0; i < argc; i++) {
          result.push_back(argv[i] == nullptr ? "" : std::string(argv[i]));
        }
        callback(result);
        return 0;
      },
      &callback, &err);
  auto result = res == SQLITE_OK;
  if (!result) {
    error = err;
    sqlite3_free(err);
  }
  return result;
}
