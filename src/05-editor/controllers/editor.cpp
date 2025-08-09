#include "editor.h"

void Editor::onClick(int x, int y) {

  switch (_mode) {
    case EditorMode::Remove: {
      auto primitives = _doc->find(x, y);
      if (!primitives.empty()) {
        notifyListeners(PrimitiveEvent{primitives.at(0).lock(), false});
      }
    } break;

    case EditorMode::Insert:
      // TODO:
      break;

    default:
      break;
  }
}