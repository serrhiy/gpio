#include <iostream>
#include <gpio/Chip.hh>
#include <ranges>

constexpr const char* chip_path = "/dev/gpiochip0";

int main(const int argc, const char* argv[]) {
  gpio::Chip chip{ chip_path };
  const auto chip_info = chip.getInfo();
  const int lines_number = chip_info.getNumLines();

  std::cout << chip_info.getName() << ' '
            << '[' << chip_info.getLabel() << "] "
            << '(' << lines_number << " lines)\n";

  for (const int index: std::views::iota(0, lines_number)) {
    const auto line_info = chip.getLineInfo(index);
    std::cout << '[' << index << "] " << line_info.getName() << '\n';
  }

  return 0;
}
