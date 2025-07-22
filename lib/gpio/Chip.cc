#include "Chip.hh"
#include "ChipInfo.hh"
#include "LineInfo.hh"

#include <gpiod.h>
#include <string>
#include <stdexcept>

void gpio::Chip::close() {
  if (!isValid()) return;
  gpiod_chip_close(chip);
  chip = nullptr;
}

void gpio::Chip::throwIfIsNotValid() const {
  if (!isValid()) {
    throw std::runtime_error{ "Chip object is not valid" };
  }
}

gpio::Chip::Chip(const std::string& path): chip{ gpiod_chip_open(path.c_str()) } {
  if (!isValid()) {
    throw std::runtime_error{ "Cannot open chip " + path };
  }
}

gpio::Chip::Chip(Chip&& other) noexcept: chip{ other.chip } {
  other.chip = nullptr;
}

gpio::Chip& gpio::Chip::operator=(Chip&& other) {
  if (this != &other) {
    close();
    chip = other.chip;
    other.chip = nullptr; 
  }
  return *this;
}

gpio::Chip::~Chip() {
  close();
}

bool gpio::Chip::isValid() const {
  return chip != nullptr;
}

std::string gpio::Chip::getPath() const {
  throwIfIsNotValid();
  return gpiod_chip_get_path(chip);
}

int gpio::Chip::getFd() const {
  throwIfIsNotValid();
  return gpiod_chip_get_fd(chip);
}

gpio::ChipInfo gpio::Chip::getInfo() {
  return ChipInfo{ chip };
}

gpio::LineInfo gpio::Chip::getLineInfo(unsigned offset) {
  return LineInfo{ chip, offset };
}

void gpio::swap(gpio::Chip& first, gpio::Chip& second) {
  using std::swap;

  swap(first.chip, second.chip);
}
