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

  LineInfo::LineInfo(gpiod_info_event* info_event):
    line_info{ gpiod_info_event_get_line_info(info_event) },
    call_destructor{ false }
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
    if (call_destructor) free();
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
    return direction_map.at(gpiod_line_info_get_direction(line_info));
  }

  LineEdge LineInfo::getEdgeDetection() const {
    throwIfIsNotValid();
    return edge_map.at(gpiod_line_info_get_edge_detection(line_info));
  }

  LineBias LineInfo::getBias() const {
    throwIfIsNotValid();
    return bias_map.at(gpiod_line_info_get_bias(line_info));
  }

  LineDrive LineInfo::getDrive() const {
    throwIfIsNotValid();
    return drive_map.at(gpiod_line_info_get_drive(line_info));
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
    return clock_map.at(gpiod_line_info_get_event_clock(line_info));
  }
}
