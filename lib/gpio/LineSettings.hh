#pragma once

#include "LineInfo.hh"

#include <gpiod.h>

namespace gpio {
  class LineSettings {
    gpiod_line_settings* line_settings;

    bool isValid() const;
    void throwIfIsNotValid() const;
    void free();

    public:
    LineSettings();

    LineSettings(const LineSettings&) = delete;
    LineSettings& operator=(const LineSettings&) = delete;

    LineSettings(LineSettings&& other) noexcept;
    LineSettings& operator=(LineSettings&& other) noexcept;

    ~LineSettings();

    void reset() const;

    int setDirection(const LineDirection direction) const;
    LineDirection getDirection() const;

    int setEdgeDetection(const LineEdge edge_detection) const;
    LineEdge getEdgeDetection() const;

    int setBias(const LineBias bias) const;
    LineBias getBias() const;

    int setDrive(const LineDrive drive) const;
    LineDrive getDrive() const;

    void setActiveLow(const bool active_low) const;
    bool getActiveLow() const;

    void setDebouncePeriod(unsigned long period) const;
    unsigned long getDebouncePeriod() const;

    int setEventClock(const LineClock clock) const;
    LineClock getEventClock() const;

    int setOutputValue(const LineValue value) const;
    LineValue getOutputValue() const;
  };
}
