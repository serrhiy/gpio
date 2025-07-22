#include "LineInfo.hh"

#include <gpiod.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace gpio {

  bool LineInfo::isValid() const {
    return line_info != nullptr;
  }

  void LineInfo::free() {
    if (!isValid()) return;
    gpiod_line_info_free(line_info);
    line_info = nullptr;
  }

  void LineInfo::throwIfIsNotValid() const {
    if (!isValid()) {
      throw std::runtime_error{ "Line info is not valid" };
    }
  }

  LineInfo::LineInfo(gpiod_chip* chip, unsigned offset):
    line_info{ gpiod_chip_get_line_info(chip, offset) }
  {
    if (!isValid()) {
      throw std::runtime_error{ "Cannot allocate line info" };
    }
  }

  LineInfo::LineInfo(LineInfo&& other) noexcept : line_info{ other.line_info } {
    other.line_info = nullptr;
  }

  LineInfo& LineInfo::operator=(LineInfo&& other) noexcept {
    if (this != &other) {
      free();
      line_info = other.line_info;
      other.line_info = nullptr;
    }
    return *this;
  }

  LineInfo::~LineInfo() {
    free();
  }

  unsigned LineInfo::getOffset() const {
    throwIfIsNotValid();
    return gpiod_line_info_get_offset(line_info);
  }

  std::string LineInfo::getName() const {
    throwIfIsNotValid();
    return gpiod_line_info_get_name(line_info);
  }

  bool LineInfo::isUsed() const {
    throwIfIsNotValid();
    return gpiod_line_info_is_used(line_info);
  }

  std::string LineInfo::getConsumer() const {
    throwIfIsNotValid();
    const char* name = gpiod_line_info_get_consumer(line_info);
    return std::string{ name == nullptr ? "" : name };
  }

  LineDirection LineInfo::getDirection() const {
    throwIfIsNotValid();
    static const std::unordered_map<gpiod_line_direction, LineDirection> map {
      { GPIOD_LINE_DIRECTION_AS_IS, LineDirection::AS_IS },
      { GPIOD_LINE_DIRECTION_INPUT, LineDirection::INPUT },
      { GPIOD_LINE_DIRECTION_OUTPUT, LineDirection::OUTPUT },
    };
    return map.at(gpiod_line_info_get_direction(line_info));
  }

  LineEdge LineInfo::getEdgeDetection() const {
    throwIfIsNotValid();
    static const std::unordered_map<gpiod_line_edge, LineEdge> map {
      { GPIOD_LINE_EDGE_NONE, LineEdge::NONE },
      { GPIOD_LINE_EDGE_RISING, LineEdge::RISING },
      { GPIOD_LINE_EDGE_FALLING, LineEdge::FALLING },
      { GPIOD_LINE_EDGE_BOTH, LineEdge::BOTH },
    };
    return map.at(gpiod_line_info_get_edge_detection(line_info));
  }

  LineBias LineInfo::getBias() const {
    throwIfIsNotValid();
    static const std::unordered_map<gpiod_line_bias, LineBias> map {
      { GPIOD_LINE_BIAS_PULL_UP, LineBias::PULL_UP },
      { GPIOD_LINE_BIAS_PULL_DOWN, LineBias::PULL_DOWN },
      { GPIOD_LINE_BIAS_DISABLED, LineBias::DISABLED },
      { GPIOD_LINE_BIAS_UNKNOWN, LineBias::UNKNOWN },
    };
    return map.at(gpiod_line_info_get_bias(line_info));
  }

  LineDrive LineInfo::getDrive() const {
    throwIfIsNotValid();
    static const std::unordered_map<gpiod_line_drive, LineDrive> map {
      { GPIOD_LINE_DRIVE_PUSH_PULL, LineDrive::PUSH_PULL },
      { GPIOD_LINE_DRIVE_OPEN_DRAIN, LineDrive::OPEN_DRAIN },
      { GPIOD_LINE_DRIVE_OPEN_SOURCE, LineDrive::OPEN_SOURCE },
    };
    return map.at(gpiod_line_info_get_drive(line_info));
  }

  bool LineInfo::isActiveLow() const {
    throwIfIsNotValid();
    return gpiod_line_info_is_active_low(line_info);
  }

  bool LineInfo::isDebounced() const {
    throwIfIsNotValid();
    return gpiod_line_info_is_debounced(line_info);
  }

  unsigned long LineInfo::getDebouncePeriod() const {
    throwIfIsNotValid();
    return gpiod_line_info_get_debounce_period_us(line_info);
  }

  LineClock LineInfo::getEventClock() const {
    throwIfIsNotValid();
    static const std::unordered_map<gpiod_line_clock, LineClock> map {
      { GPIOD_LINE_CLOCK_MONOTONIC, LineClock::MONOTONIC },
      { GPIOD_LINE_CLOCK_HTE, LineClock::HTE },
      { GPIOD_LINE_CLOCK_REALTIME, LineClock::REALTIME },
    };
    return map.at(gpiod_line_info_get_event_clock(line_info));
  }
}
