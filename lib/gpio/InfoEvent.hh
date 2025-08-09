#pragma once

#include "LineInfo.hh"

#include <cstdint>
#include <gpiod.h>
#include <stdexcept>
#include <unordered_map>

namespace gpio {
  enum class InfoEventType {
    LINE_REQUESTED,
    LINE_RELEASED,
    LINE_CONFIG_CHANGED
  };

  class InfoEvent {
    gpiod_info_event* info_event;

    bool isValid() const;
    void throwIfIsNotValid() const;
    void free();
  public:
    InfoEvent() = delete;
    explicit InfoEvent(gpiod_chip* chip);

    InfoEvent(const InfoEvent&) = delete;
    InfoEvent& operator=(const InfoEvent&) = delete;

    InfoEvent(InfoEvent&& other);
    InfoEvent& operator=(InfoEvent&& other);

    ~InfoEvent();

    InfoEventType getEventType() const;
    std::uint64_t getTimeStamp() const;
    LineInfo getLineInfo() const;
  };
}
