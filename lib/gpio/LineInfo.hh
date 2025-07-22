#pragma once

#include <gpiod.h>
#include <string>
#include <unordered_map>

namespace gpio {
  enum class LineValue { ERROR, INACTIVE, ACTIVE };
  enum class LineDirection { AS_IS, INPUT, OUTPUT };
  enum class LineEdge { NONE, RISING, FALLING, BOTH };
  enum class LineBias { AS_IS, UNKNOWN, DISABLED, PULL_UP, PULL_DOWN };
  enum class LineDrive { PUSH_PULL, OPEN_DRAIN, OPEN_SOURCE };
  enum class LineClock { MONOTONIC, REALTIME, HTE };

  inline const std::unordered_map<gpiod_line_direction, LineDirection> direction_map {
    { GPIOD_LINE_DIRECTION_AS_IS, LineDirection::AS_IS },
    { GPIOD_LINE_DIRECTION_INPUT, LineDirection::INPUT },
    { GPIOD_LINE_DIRECTION_OUTPUT, LineDirection::OUTPUT },
  };

  inline const std::unordered_map<gpiod_line_edge, LineEdge> edge_map {
    { GPIOD_LINE_EDGE_NONE, LineEdge::NONE },
    { GPIOD_LINE_EDGE_RISING, LineEdge::RISING },
    { GPIOD_LINE_EDGE_FALLING, LineEdge::FALLING },
    { GPIOD_LINE_EDGE_BOTH, LineEdge::BOTH },
  };

  inline const std::unordered_map<gpiod_line_bias, LineBias> bias_map {
    { GPIOD_LINE_BIAS_PULL_UP, LineBias::PULL_UP },
    { GPIOD_LINE_BIAS_PULL_DOWN, LineBias::PULL_DOWN },
    { GPIOD_LINE_BIAS_DISABLED, LineBias::DISABLED },
    { GPIOD_LINE_BIAS_UNKNOWN, LineBias::UNKNOWN },
  };

  inline const std::unordered_map<gpiod_line_drive, LineDrive> drive_map {
    { GPIOD_LINE_DRIVE_PUSH_PULL, LineDrive::PUSH_PULL },
    { GPIOD_LINE_DRIVE_OPEN_DRAIN, LineDrive::OPEN_DRAIN },
    { GPIOD_LINE_DRIVE_OPEN_SOURCE, LineDrive::OPEN_SOURCE },
  };

  inline const std::unordered_map<gpiod_line_clock, LineClock> clock_map {
    { GPIOD_LINE_CLOCK_MONOTONIC, LineClock::MONOTONIC },
    { GPIOD_LINE_CLOCK_HTE, LineClock::HTE },
    { GPIOD_LINE_CLOCK_REALTIME, LineClock::REALTIME },
  };

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
