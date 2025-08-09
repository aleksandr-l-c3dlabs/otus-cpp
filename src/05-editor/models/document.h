#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <vector>

#include "primitives.h"

namespace fs = std::filesystem;

/// @brief Document with vector primitives
class Document {
  using Ptr = Primitive::Ptr;
  using WeakPtr = Primitive::WeakPtr;
  using ConstWeakPtr = Primitive::ConstWeakPtr;

 public:
  /// @brief Construct
  Document() {}
  /// @brief Destructor
  ~Document() = default;

  /// @brief Add the new primitive to the document
  /// @param[in] primitive Primitive pointer
  void add(Ptr primitive);

  /// @brief Find a exists primitives on selected position
  /// @param[in] x Viewport X position
  /// @param[in] y Viewport Y position
  /// @return A list of primitives that are on this position
  std::vector<WeakPtr> find(int x, int y) const;

  /// @brief Remove exists primitive
  /// @param[in] primitive Primitive pointer
  /// @return Removing result (false if the primitive is not found)
  bool remove(WeakPtr primitive);

  /// @brief Get all document primitives
  /// @details Need for viewport rendering
  /// @return A list of primitive pointers
  std::vector<ConstWeakPtr> getPrimitives() const;

  /// @brief Get current document file path
  /// @return A file path, if it was set previously
  std::optional<fs::path> getFilepath() const { return _filepath; }

  /// @brief Save document to a selected file
  /// @param[in] path File path
  /// @return Saving result
  bool exportFile(fs::path path);

  /// @brief Load document from a selected file
  /// @param[in] path File path
  /// @return A document pointer
  static std::unique_ptr<Document> importFile(fs::path path);

 private:
  std::optional<fs::path> _filepath;
  std::vector<Ptr> _primitives;
};