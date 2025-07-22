#pragma once

#include "ChipInfo.hh"

#include <gpiod.h>
#include <string>

namespace gpio
{
  class Chip {
    gpiod_chip* chip;

    void close();
    void throwIfIsNotValid() const;

    public:
    Chip() = delete;
    explicit Chip(const std::string& path);

    Chip(const Chip& other) = delete;
    Chip& operator=(const Chip& other) = delete;

    Chip(Chip&& other) noexcept;
    Chip& operator=(Chip&& other);

    ~Chip();

    friend void swap(Chip& first, Chip& second);

    bool isValid() const;

    std::string getPath() const;

    int getFd() const;

    ChipInfo getInfo();
  };

  void swap(Chip& first, Chip& second);
}
