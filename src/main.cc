#include <iostream>
#include <filesystem>

#include <gpio/utils.hh>
#include <gpio/Chip.hh>

namespace fs = std::filesystem;

int main(const int argc, const char* argv[]) {
  const fs::path chip_path = gpio::getDefaultChipPath();

  return 0;
}
