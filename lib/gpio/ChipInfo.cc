#include "ChipInfo.hh"

#include <gpiod.h>
#include <stdexcept>

gpio::ChipInfo::ChipInfo(gpiod_chip* chip): chip_info{ gpiod_chip_get_info(chip) } {
  if (!isValid()) {
    throw std::runtime_error{ "Cannot allocate chip info" };
  }
}

gpio::ChipInfo::ChipInfo(ChipInfo&& other): chip_info{ other.chip_info } {
  other.chip_info = nullptr;
}

gpio::ChipInfo& gpio::ChipInfo::operator=(ChipInfo&& other) {
  if (this != &other) {
    free();
    chip_info = other.chip_info;
    other.chip_info = nullptr;
  }
  return *this;
}

gpio::ChipInfo::~ChipInfo() {
  free();
}

std::string gpio::ChipInfo::getName() const {
  throwIfIsNotValid();
  return gpiod_chip_info_get_name(chip_info);
}

std::string gpio::ChipInfo::getLabel() const {
  throwIfIsNotValid();
  return gpiod_chip_info_get_label(chip_info);
}

size_t gpio::ChipInfo::getNumLines() const {
  throwIfIsNotValid();
  return gpiod_chip_info_get_num_lines(chip_info);
}

bool gpio::ChipInfo::isValid() const {
  return chip_info != nullptr;
}

void gpio::ChipInfo::free() {
  if (!isValid()) return;
  gpiod_chip_info_free(chip_info);
  chip_info = nullptr;
}

void gpio::ChipInfo::throwIfIsNotValid() const {
  if (!isValid()) {
    throw std::runtime_error{ "Chip info is not valid" };
  }
}
