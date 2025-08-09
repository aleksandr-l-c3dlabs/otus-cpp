#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <vector>

#include "editor.h"
#include "listener.h"
#include "models/document.h"
#include "views/viewport.h"

namespace fs = std::filesystem;

/**
 * @brief Global application controller as a singleton
 */
class ApplicationController : public EventListener<PrimitiveEvent> {
 private:
  /// @brief Private constructor for singleton
  ApplicationController();

  // disable coping and assigning
  ApplicationController(const ApplicationController&) = delete;
  ApplicationController& operator=(const ApplicationController&) = delete;

 public:
  /// @brief Get singleton instance
  /// @return Application control object
  static ApplicationController& instance();

  /// @brief Update viewport size
  /// @param width New viewport width
  /// @param height New viewport height
  void resize(int width, int height);

  /// @brief Open document file
  /// @details If there isn't a path parameter then it create a new document
  /// @param[in] path File path of an opening document
  void openDocument(std::optional<fs::path> path = {});

  /// @brief Close document
  /// @param[in] index Index of closing document or the current will be used
  void closeDocument(std::optional<std::size_t> index = {});

  /// @brief Save document file
  /// @param[in] path File path of a saving document
  /// @param[in] index Index of saving document or the current will be used
  void saveDocument(const fs::path& path,
                    std::optional<std::size_t> index = {});

  /// @brief Switch between opened documents
  /// @param[in] index Document index
  void switchDocument(std::size_t index);

  /// @brief Select adding primitive type
  /// @param[in] type Primitive type
  void setPrimitiveType(const PrimitiveType& type);

  /// @brief Select a mode of editor
  /// @details May be 'insert' for adding the new primitives or 'remove' for deleting exists primitives
  /// @param[in] mode Editor mode
  void setEditorMode(const EditorMode& mode);

  /// @brief Fix user click event on the viewport
  /// @param[in] x Viewport X coordinate
  /// @param[in] y Viewport Y coordinate
  void onViewportClick(int x, int y);

 private:
  /// @brief Update viewport (redraw)
  void update();

  /// @brief Event listener from primitive editor
  /// @param[in] event Event struct with a new added primitive or primitive for removing
  void onEvent(const PrimitiveEvent& event) override;

 private:
  /// @brief Current opened document index
  std::size_t currentDocumentIndex = 0;
  /// @brief A list of all opened documents
  std::vector<std::unique_ptr<Document>> documents;
  /// @brief Viewport object
  Viewport viewport;
  /// @brief Editor object
  Editor editor;
};