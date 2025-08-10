#pragma once

#include <filesystem>
#include <vector>

namespace gpio {
  std::filesystem::path getDefaultChipPath();
  std::vector<std::filesystem::path> getChipsPath();
}