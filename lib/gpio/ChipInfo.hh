#pragma once

#include <gpiod.h>
#include <string>

namespace gpio {
  class ChipInfo {
    gpiod_chip_info* chip_info;

    bool isValid() const;
    void free();
    void throwIfIsNotValid() const;
  public:
    ChipInfo() = delete;
    ChipInfo(gpiod_chip* chip);

    ChipInfo(const ChipInfo&) = delete;
    ChipInfo& operator=(const ChipInfo&) = delete;

    ChipInfo(ChipInfo&& other) noexcept;
    ChipInfo& operator=(ChipInfo&& other) noexcept;

    ~ChipInfo();

    std::string getName() const;
    std::string getLabel() const;
    size_t getNumLines() const;
  };
}
