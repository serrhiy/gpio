#include "InfoEvent.hh"
#include "LineInfo.hh"

#include <gpiod.h>
#include <utility>
#include <unordered_map>

namespace gpio {
  bool InfoEvent::isValid() const {
    return info_event != nullptr;
  }

  void InfoEvent::throwIfIsNotValid() const {
    if (!isValid()) {
      throw std::runtime_error{ "Info event is not valid" };
    }
  }

  void InfoEvent::free() {
    if (!isValid()) return;
    gpiod_info_event_free(info_event);
    info_event = nullptr;
  }

  InfoEvent::InfoEvent(gpiod_chip* chip)
    : info_event{ gpiod_chip_read_info_event(chip) }
  {
    if (!isValid()) {
      throw std::runtime_error{ "Cannot allocate info event" };
    }
  }

  InfoEvent::InfoEvent(InfoEvent&& other)
    : info_event{ other.info_event }
  {
    other.info_event = nullptr;
  }

  InfoEvent& InfoEvent::operator=(InfoEvent&& other) {
    if (this != &other) {
      free();
      std::swap(info_event, other.info_event);
    }
    return *this;
  }

  InfoEvent::~InfoEvent() {
    free();
  }

  InfoEventType InfoEvent::getEventType() const {
    throwIfIsNotValid();
    static const std::unordered_map<gpiod_info_event_type, InfoEventType> map {
      { GPIOD_INFO_EVENT_LINE_REQUESTED, InfoEventType::LINE_REQUESTED },
      { GPIOD_INFO_EVENT_LINE_RELEASED, InfoEventType::LINE_RELEASED },
      { GPIOD_INFO_EVENT_LINE_CONFIG_CHANGED, InfoEventType::LINE_CONFIG_CHANGED }
    };
    return map.at(gpiod_info_event_get_event_type(info_event));
  }

  std::uint64_t InfoEvent::getTimeStamp() const {
    throwIfIsNotValid();
    return gpiod_info_event_get_timestamp_ns(info_event);
  }

  LineInfo InfoEvent::getLineInfo() const {
    throwIfIsNotValid();
    return LineInfo{ info_event };
  }
}
