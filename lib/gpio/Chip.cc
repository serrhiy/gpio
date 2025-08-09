#include "Chip.hh"
#include "ChipInfo.hh"
#include "LineInfo.hh"

#include <filesystem>
#include <gpiod.h>
#include <stdexcept>
#include <string>

namespace gpio {
  void Chip::close() {
    if (!isValid()) return;
    gpiod_chip_close(chip);
    chip = nullptr;
  }

  void Chip::throwIfIsNotValid() const {
    if (!isValid()) {
      throw std::runtime_error{ "Chip object is not valid" };
    }
  }

  Chip::Chip(const std::filesystem::path& path): chip{ nullptr } {
    if (!std::filesystem::exists(path)) {
      throw std::runtime_error{ "File does not exist" };
    }
    chip = gpiod_chip_open(path.c_str());
    if (!isValid()) {
      throw std::runtime_error{ "Cannot open chip " + path.string() };
    }
  }

  Chip::Chip(Chip&& other) noexcept: chip{ other.chip } {
    other.chip = nullptr;
  }

  Chip& gpio::Chip::operator=(Chip&& other) noexcept {
    if (this != &other) {
      close();
      chip = other.chip;
      other.chip = nullptr;
    }
    return *this;
  }

  Chip::~Chip() { close(); }

  bool Chip::isValid() const { return chip != nullptr; }

  std::string Chip::getPath() const {
    throwIfIsNotValid();
    return gpiod_chip_get_path(chip);
  }

  int Chip::getFd() const {
    throwIfIsNotValid();
    return gpiod_chip_get_fd(chip);
  }

  ChipInfo Chip::getInfo() { return ChipInfo{ chip }; }

  LineInfo Chip::getLineInfo(unsigned offset) {
    return LineInfo{ chip, offset };
  }

  InfoEvent Chip::getInfoEvent() { return InfoEvent{ chip }; }

  void swap(Chip& first, Chip& second) {
    using std::swap;

    swap(first.chip, second.chip);
  }
}
