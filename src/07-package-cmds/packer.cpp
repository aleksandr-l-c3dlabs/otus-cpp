#include "packer.h"

void Packer::onEvent(const InputCommandEvent& event) {
  if (event.command == "{") {
    if (level == 0 && !collection.empty())
      packCollection();
    level++;
  } else if (event.command == "}") {
    if (level == 0)
      return;
    level--;
    if (level == 0 && !collection.empty())
      packCollection();
  } else {
    if (!event.command.empty()) {
      if (collection.empty())
        created = std::time(nullptr);
      collection.push_back(event.command);
    }
    if (level == 0 && collection.size() > 0 &&
        (event.interrupted || collection.size() == pack_size))
      packCollection();
  }
}

void Packer::packCollection() {
  PackEvent pack{created, std::move(collection)};
  notifyListeners(pack);

  collection = std::vector<std::string>();
  collection.reserve(pack_size);
}