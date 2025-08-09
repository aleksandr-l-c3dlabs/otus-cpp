#include "document.h"

#include <algorithm>
#include <iostream>

void Document::add(Document::Ptr primitive) {
  _primitives.push_back(primitive);
}

std::vector<Document::WeakPtr> Document::find(int x, int y) const {
  std::vector<std::weak_ptr<Primitive>> result;
  for (const auto& pr : _primitives) {
    if (pr->contain(x, y))
      result.push_back(pr);
  }
  return result;
}

bool Document::remove(Document::WeakPtr primitive) {
  bool result = false;
  auto ptr = primitive.lock();
  if (ptr) {
    _primitives.erase(std::remove_if(_primitives.begin(), _primitives.end(),
                                     [&result, ptr](auto p) {
                                       if (p == ptr) {
                                         result = true;
                                         return true;
                                       }
                                       return false;
                                     }),
                      _primitives.end());
  }
  return result;
}

std::vector<Document::ConstWeakPtr> Document::getPrimitives() const {
  std::vector<ConstWeakPtr> result;
  result.reserve(_primitives.size());
  for (const auto& pr : _primitives)
    result.push_back(pr);
  return result;
}

bool Document::exportFile(fs::path path) {
  std::cout << "Export document to file '" << path.string() << "'" << std::endl;
  if (!_filepath.has_value()) {
    _filepath = path;
    return true;
  }
  return false;
}

std::unique_ptr<Document> Document::importFile(fs::path path) {
  std::cout << "Import document from file '" << path.string() << "'"
            << std::endl;
  auto doc = std::make_unique<Document>();
  doc->_filepath = path;
  return doc;
}