#pragma once

#include "ChipInfo.hh"
#include "InfoEvent.hh"
#include "LineInfo.hh"
#include "LineRequest.hh"
#include "RequestConfig.hh"
#include "LineConfig.hh"

#include <filesystem>
#include <gpiod.h>
#include <string>

namespace gpio {
  class Chip {
    gpiod_chip* chip;

    bool isValid() const;
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

    std::string getPath() const;

    int getFd() const;

    ChipInfo getInfo() const;
    LineInfo getLineInfo(unsigned offset) const;
    InfoEvent getInfoEvent() const;
    LineRequest requestLines(const RequestConfig& request_config, const LineConfig& line_config) const;
  };

  void swap(Chip& first, Chip& second);
}
