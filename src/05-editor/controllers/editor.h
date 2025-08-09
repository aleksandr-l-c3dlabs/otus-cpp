#pragma once

#include "models/document.h"
#include "models/primitives.h"
#include "provider.h"

/// @brief Available editor modes
enum class EditorMode {
  /// @brief Insert the new primitives
  Insert,
  /// @brief Remove existing primitives
  Remove,
};

/// @brief Editor event struct
struct PrimitiveEvent {
  /// @brief The primitive
  Primitive::Ptr primitive;
  /// @brief Need insert in document or remove from document
  bool isNew;
};

/// @brief Editor control
/// @details Processes user clicks on the viewport and, depending on them, generates an event for adding/removing primitives
class Editor : public EventProvider<PrimitiveEvent> {
 public:
  /// @brief Set current document for searching exists primitives
  /// @param[in] doc Current document
  void setDocument(const Document* doc = nullptr) { _doc = doc; }

  /// @brief Set a type for the new primitives
  /// @param[in] type Primitive type
  void setPrimitiveType(const PrimitiveType& type) { _type = type; }

  /// @brief Set an editor mode
  /// @param[in] mode Editor mode
  void setMode(const EditorMode& mode) { _mode = mode; }

  /// @brief User viewport click callback
  /// @param[in] x Viewport X position
  /// @param[in] y Viewport Y position
  void onClick(int x, int y);

 private:
  EditorMode _mode;
  PrimitiveType _type;
  const Document* _doc = nullptr;
};
