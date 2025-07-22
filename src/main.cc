#include <iostream>
#include <gpio/Chip.hh>

constexpr const char* chip_path = "/dev/gpiochip0";

int main(const int argc, const char* argv[]) {
  gpio::Chip chip{ chip_path };
  const auto info = chip.getInfo();
  std::cout << info.getName() << ' '
            << '[' << info.getLabel() << "] "
            << '(' << info.getNumLines() << " lines)\n";
  return 0;
}
