#pragma once

#include <gpiod.h>
#include <string>

namespace gpio {
  class ChipInfo {
    gpiod_chip_info* chip_info;

    void free();
    void throwIfIsNotValid() const;

    public:
    ChipInfo() = delete;
    ChipInfo(gpiod_chip* chip);

    ChipInfo(const ChipInfo&) = delete;
    ChipInfo& operator=(const ChipInfo&) = delete;

    ChipInfo(ChipInfo&& other);
    ChipInfo& operator=(ChipInfo&& other);

    ~ChipInfo();

    std::string getName() const;
    std::string getLabel() const;
    size_t getNumLines() const;
    bool isValid() const;
  };
}
