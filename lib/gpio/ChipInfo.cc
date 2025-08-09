#include "ChipInfo.hh"

#include <gpiod.h>
#include <stdexcept>

namespace gpio {
  ChipInfo::ChipInfo(gpiod_chip* chip): chip_info{ gpiod_chip_get_info(chip) } {
    if (!isValid()) {
      throw std::runtime_error{ "Cannot allocate chip info" };
    }
  }

  ChipInfo::ChipInfo(ChipInfo&& other) noexcept: chip_info{ other.chip_info } {
    other.chip_info = nullptr;
  }

  ChipInfo& ChipInfo::operator=(ChipInfo&& other) noexcept {
    if (this != &other) {
      free();
      chip_info = other.chip_info;
      other.chip_info = nullptr;
    }
    return *this;
  }

  ChipInfo::~ChipInfo() { free(); }

  std::string ChipInfo::getName() const {
    throwIfIsNotValid();
    return gpiod_chip_info_get_name(chip_info);
  }

  std::string ChipInfo::getLabel() const {
    throwIfIsNotValid();
    return gpiod_chip_info_get_label(chip_info);
  }

  size_t ChipInfo::getNumLines() const {
    throwIfIsNotValid();
    return gpiod_chip_info_get_num_lines(chip_info);
  }

  bool ChipInfo::isValid() const { return chip_info != nullptr; }

  void ChipInfo::free() {
    if (!isValid()) return;
    gpiod_chip_info_free(chip_info);
    chip_info = nullptr;
  }

  void ChipInfo::throwIfIsNotValid() const {
    if (!isValid()) {
      throw std::runtime_error{ "Chip info is not valid" };
    }
  }
}
