#include "application.h"

#include <algorithm>

ApplicationController::ApplicationController() {
  editor.addListener(std::shared_ptr<ApplicationController>(this));
}

ApplicationController& ApplicationController::instance() {
  static ApplicationController app;
  return app;
}

void ApplicationController::resize(int width, int height) {
  viewport.resize(width, height);
  update();
}

void ApplicationController::openDocument(std::optional<fs::path> path) {
  auto doc = std::make_unique<Document>();
  if (path.has_value()) {
    doc = Document::importFile(path.value());
  }
  currentDocumentIndex = documents.size();
  documents.push_back(std::move(doc));
}

void ApplicationController::closeDocument(std::optional<std::size_t> i) {
  auto index = i.value_or(currentDocumentIndex);
  documents.erase(documents.begin() + index);
  if (currentDocumentIndex == index) {
    switchDocument(std::max(index - 1, std::size_t(0)));
  } else if (currentDocumentIndex > index) {
    currentDocumentIndex -= 1;
  }
}

void ApplicationController::saveDocument(const fs::path& path,
                                         std::optional<std::size_t> index) {
  documents.at(index.value_or(currentDocumentIndex))->exportFile(path);
}

void ApplicationController::switchDocument(std::size_t index) {
  if (index < documents.size()) {
    currentDocumentIndex = index;
    update();
  }
}

void ApplicationController::setPrimitiveType(const PrimitiveType& type) {
  editor.setPrimitiveType(type);
}

void ApplicationController::setEditorMode(const EditorMode& mode) {
  editor.setMode(mode);
}

void ApplicationController::onViewportClick(int x, int y) {
  editor.onClick(x, y);
}

void ApplicationController::update() {
  if (documents.empty()) {
    viewport.clear();
    editor.setDocument(nullptr);
  } else {
    viewport.draw(documents.at(currentDocumentIndex)->getPrimitives());
    editor.setDocument(documents.at(currentDocumentIndex).get());
  }
}

void ApplicationController::onEvent(const PrimitiveEvent& event) {
  if (documents.empty())
    return;
  auto doc = documents.at(currentDocumentIndex).get();
  if (event.isNew) {
    doc->add(event.primitive);
  } else {
    doc->remove(event.primitive);
  }
}