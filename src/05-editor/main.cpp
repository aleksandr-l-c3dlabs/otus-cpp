#include <cstddef>
#include <string>
#include <unordered_map>
#include "controllers/application.h"

std::string selectPathDialog() {
  return "/some/file";
}

PrimitiveType getPrimitiveTypeByIndex(std::size_t index) {
  static std::unordered_map<std::size_t, PrimitiveType> types{
      {0, PrimitiveType::Line},
      {1, PrimitiveType::Rectangle},
      {2, PrimitiveType::Circle},
  };
  auto it = types.find(index);
  return it != types.end() ? it->second : PrimitiveType::Line;
}

EditorMode getEditorModeByIndex(std::size_t index) {
  static std::unordered_map<std::size_t, EditorMode> modes{
      {0, EditorMode::Insert},
      {1, EditorMode::Remove},
  };
  auto it = modes.find(index);
  return it != modes.end() ? it->second : EditorMode::Insert;
}

void clickNewDocument() {
  ApplicationController::instance().openDocument();
}

void clickCloseDocument() {
  ApplicationController::instance().closeDocument();
}

void clickImportFile() {
  auto path = selectPathDialog();
  ApplicationController::instance().openDocument(path);
}

void clickExportFile() {
  auto path = selectPathDialog();
  ApplicationController::instance().saveDocument(path);
}

void clickSwitchDocuments(std::size_t index) {
  ApplicationController::instance().switchDocument(index);
}

void selectPrimitive(std::size_t id) {
  auto type = getPrimitiveTypeByIndex(id);
  ApplicationController::instance().setPrimitiveType(type);
}

void selectMode(std::size_t id) {
  auto mode = getEditorModeByIndex(id);
  ApplicationController::instance().setEditorMode(mode);
}

void clickViewport(int x, int y) {
  ApplicationController::instance().onViewportClick(x, y);
}

void resizeViewport(int w, int h) {
  ApplicationController::instance().resize(w, h);
}

int main() {
  return 0;
}