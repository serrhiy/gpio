#pragma once

#include <gpiod.h>
#include <string>

namespace gpio {
  enum class LineValue { ERROR, INACTIVE, ACTIVE };
  enum class LineDirection { AS_IS, INPUT, OUTPUT };
  enum class LineEdge { NONE, RISING, FALLING, BOTH };
  enum class LineBias { AS_IS, UNKNOWN, DISABLED, PULL_UP, PULL_DOWN };
  enum class LineDrive { PUSH_PULL, OPEN_DRAIN, OPEN_SOURCE };
  enum class LineClock { MONOTONIC, REALTIME, HTE };

  class LineInfo {
    gpiod_line_info* line_info;
    const bool call_destructor = true;

    bool isValid() const;
    void free();
    void throwIfIsNotValid() const;

  public:
    LineInfo() = delete;
    LineInfo(gpiod_chip* chip, unsigned offset);
    explicit LineInfo(gpiod_info_event* info_event);

    LineInfo(const LineInfo&) = delete;
    LineInfo& operator=(const LineInfo&) = delete;

    LineInfo(LineInfo&& other) noexcept;
    LineInfo& operator=(LineInfo&& other) noexcept;

    ~LineInfo();

    unsigned getOffset() const;
    std::string getName() const;
    bool isUsed() const;
    std::string getConsumer() const;
    LineDirection getDirection() const;
    LineEdge getEdgeDetection() const;
    LineBias getBias() const;
    LineDrive getDrive() const;
    bool isActiveLow() const;
    bool isDebounced() const;
    unsigned long getDebouncePeriod() const;
    LineClock getEventClock() const;
  };
}
