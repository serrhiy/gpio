#include "LineSettings.hh"

#include <gpiod.h>
#include <stdexcept>
#include <utility>
#include <unordered_map>

namespace gpio {

  bool LineSettings::isValid() const {
    return line_settings != nullptr;
  }

  void LineSettings::throwIfIsNotValid() const {
    if (!isValid()) {
      throw std::runtime_error{ "Line Settings object is not valid" };
    }
  }

  void LineSettings::free() {
    if (!isValid()) return;
    gpiod_line_settings_free(line_settings);
    line_settings = nullptr;
  }

  LineSettings::LineSettings():
    line_settings{ gpiod_line_settings_new() }
  {
    if (!isValid()) {
      throw std::runtime_error{ "Cannot allocate line settings" };
    }
  }

  LineSettings::LineSettings(gpiod_line_settings* line_settings):
    line_settings{ line_settings }
  {

  }

  LineSettings::LineSettings(LineSettings&& other) noexcept:
    line_settings{ other.line_settings }
  {
    other.line_settings = nullptr;
  }

  LineSettings &LineSettings::operator=(LineSettings&& other) noexcept {
    if (this != &other) {
      free();
      std::swap(line_settings, other.line_settings);
    }
    return *this;
  }

  LineSettings::~LineSettings() {
    free();
  }

  void LineSettings::reset() const {
    throwIfIsNotValid();
    gpiod_line_settings_reset(line_settings);
  }

  int LineSettings::setDirection(const LineDirection direction) const {
    throwIfIsNotValid();
    static const std::unordered_map<LineDirection, gpiod_line_direction> map {
      { LineDirection::AS_IS , GPIOD_LINE_DIRECTION_AS_IS },
      { LineDirection::INPUT, GPIOD_LINE_DIRECTION_INPUT },
      { LineDirection::OUTPUT, GPIOD_LINE_DIRECTION_OUTPUT },
    };
    return gpiod_line_settings_set_direction(line_settings, map.at(direction));
  }

  LineDirection LineSettings::getDirection() const {
    throwIfIsNotValid();
    return direction_map.at(gpiod_line_settings_get_direction(line_settings));
  }

  int LineSettings::setEdgeDetection(const LineEdge edge_detection) const {
    throwIfIsNotValid();
    static const std::unordered_map<LineEdge, gpiod_line_edge> map {
      { LineEdge::NONE, GPIOD_LINE_EDGE_NONE },
      { LineEdge::RISING, GPIOD_LINE_EDGE_RISING },
      { LineEdge::FALLING, GPIOD_LINE_EDGE_FALLING },
      { LineEdge::BOTH, GPIOD_LINE_EDGE_BOTH },
    };
    return gpiod_line_settings_set_edge_detection(line_settings, map.at(edge_detection));
  }

  LineEdge LineSettings::getEdgeDetection() const {
    throwIfIsNotValid();
    return edge_map.at(gpiod_line_settings_get_edge_detection(line_settings));
  }

  int LineSettings::setBias(const LineBias bias) const {
    throwIfIsNotValid();
    static const std::unordered_map<LineBias, gpiod_line_bias> map {
      { LineBias::PULL_UP, GPIOD_LINE_BIAS_PULL_UP },
      { LineBias::PULL_DOWN, GPIOD_LINE_BIAS_PULL_DOWN },
      { LineBias::DISABLED, GPIOD_LINE_BIAS_DISABLED },
      { LineBias::UNKNOWN, GPIOD_LINE_BIAS_UNKNOWN },
    };
    return gpiod_line_settings_set_bias(line_settings, map.at(bias));
  }

  LineBias LineSettings::getBias() const {
    throwIfIsNotValid();
    return bias_map.at(gpiod_line_settings_get_bias(line_settings));
  };

  int LineSettings::setDrive(const LineDrive drive) const {
    throwIfIsNotValid();
    static const std::unordered_map<LineDrive, gpiod_line_drive> map {
      { LineDrive::PUSH_PULL, GPIOD_LINE_DRIVE_PUSH_PULL },
      { LineDrive::OPEN_DRAIN, GPIOD_LINE_DRIVE_OPEN_DRAIN },
      { LineDrive::OPEN_SOURCE, GPIOD_LINE_DRIVE_OPEN_SOURCE},
    };
    return gpiod_line_settings_set_drive(line_settings, map.at(drive));
  }

  LineDrive LineSettings::getDrive() const {
    throwIfIsNotValid();
    return drive_map.at(gpiod_line_settings_get_drive(line_settings));
  }

  void LineSettings::setActiveLow(const bool active_low) const {
    throwIfIsNotValid();
    gpiod_line_settings_set_active_low(line_settings, active_low);
  }

  bool LineSettings::getActiveLow() const {
    throwIfIsNotValid();
    return gpiod_line_settings_get_active_low(line_settings);
  }

  void LineSettings::setDebouncePeriod(unsigned long period) const {
    throwIfIsNotValid();
    gpiod_line_settings_set_debounce_period_us(line_settings, period);
  }

  unsigned long LineSettings::getDebouncePeriod() const {
    throwIfIsNotValid();
    return gpiod_line_settings_get_debounce_period_us(line_settings);
  }

  int LineSettings::setEventClock(const LineClock clock) const {
    throwIfIsNotValid();
    static const std::unordered_map<LineClock, gpiod_line_clock> map {
      { LineClock::MONOTONIC, GPIOD_LINE_CLOCK_MONOTONIC },
      { LineClock::HTE, GPIOD_LINE_CLOCK_HTE },
      { LineClock::REALTIME, GPIOD_LINE_CLOCK_REALTIME },
    };
    return gpiod_line_settings_set_event_clock(line_settings, map.at(clock));
  }

  LineClock LineSettings::getEventClock() const {
    throwIfIsNotValid();
    return clock_map.at(gpiod_line_settings_get_event_clock(line_settings));
  }

  int LineSettings::setOutputValue(const LineValue value) const {
    throwIfIsNotValid();
    return gpiod_line_settings_set_output_value(line_settings, line_value_reverse_map.at(value));
  }

  LineValue LineSettings::getOutputValue() const {
    throwIfIsNotValid();
    return line_value_map.at(gpiod_line_settings_get_output_value(line_settings));
  }
}
