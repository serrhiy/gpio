#include "utils.hh"

#include <filesystem>
#include <string>
#include <ranges>
#include <algorithm>
#include <vector>

std::vector<std::filesystem::path> gpio::getChipsPath() {
  namespace fs = std::filesystem;
  using std::views::filter, std::views::transform;

  static const fs::path root_directory = "/dev";
  static const auto predicate = [](const fs::directory_entry& entry) {
    return (
      entry.is_character_file() &&
      entry.path().filename().string().starts_with("gpiochip")
    );
  };
  
  auto chips = fs::directory_iterator{ root_directory }
    | filter(predicate)
    | transform([](const fs::directory_entry& entry) { return entry.path(); });

  std::vector<fs::path> result;
  std::ranges::copy(chips, std::back_inserter(result));
  std::ranges::sort(result);
  return result;
}

std::filesystem::path gpio::getDefaultChipPath() {
  const auto chips = getChipsPath();
  if (chips.empty()) {
    throw std::runtime_error{ "Chips do no finded" };
  }
  return chips[0];
}