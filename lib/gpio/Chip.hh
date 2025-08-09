#pragma once

#include "ChipInfo.hh"
#include "InfoEvent.hh"
#include "LineInfo.hh"

#include <filesystem>
#include <gpiod.h>
#include <string>

namespace gpio {
  class Chip {
    gpiod_chip* chip;

    void close();
    void throwIfIsNotValid() const;
  public:
    Chip() = delete;
    explicit Chip(const std::filesystem::path& path);

    Chip(const Chip& other) = delete;
    Chip& operator=(const Chip& other) = delete;

    Chip(Chip&& other) noexcept;
    Chip& operator=(Chip&& other) noexcept;

    ~Chip();

    friend void swap(Chip& first, Chip& second);

    bool isValid() const;

    std::string getPath() const;

    int getFd() const;

    ChipInfo getInfo();
    LineInfo getLineInfo(unsigned offset);
    InfoEvent getInfoEvent();
  };

  void swap(Chip& first, Chip& second);
}
