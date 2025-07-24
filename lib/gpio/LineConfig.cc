#include "LineConfig.hh"
#include "LineSettings.hh"

#include <gpiod.h>
#include <stdexcept>
#include <memory>

namespace gpio {

  void LineConfig::free() {
    if (!isValid()) return;
    gpiod_line_config_free(line_config);
    line_config = nullptr;
  }

  bool LineConfig::isValid() const {
    return line_config != nullptr;
  }

  void LineConfig::throwIfIsNotValid() const {
    if (!isValid()) {
      throw std::runtime_error{ "Line Config object is not valid" };
    }
  }

  LineConfig::LineConfig():
    line_config{ gpiod_line_config_new() }
  {
    if (!isValid()) {
      throw std::runtime_error{ "Cannot allocate line config" };
    }
  }

  LineConfig::LineConfig(LineConfig&& other) noexcept:
    line_config{ other.line_config }
  {
    other.line_config = nullptr;
  }

  LineConfig& gpio::LineConfig::operator=(LineConfig &&other) noexcept {
    if (this != &other) {
      free();
      std::swap(line_config, other.line_config);
    }
    return *this;
  }

  LineConfig::~LineConfig() {
    free();
  }

  void LineConfig::reset() const {
    throwIfIsNotValid();
    gpiod_line_config_reset(line_config);
  }

  void LineConfig::addLineSettings(unsigned int offset, LineSettings& line_settings) const {
    throwIfIsNotValid();
    return addLineSettings(std::vector{ offset }, line_settings);
  }

  LineSettings LineConfig::getLineSettings(const unsigned int offset) const {
    throwIfIsNotValid();
    gpiod_line_settings* line_settings = gpiod_line_config_get_line_settings(line_config, offset);
    return LineSettings{ line_settings };
  }

  void LineConfig::setOutputValue(const LineValue value) const {
    throwIfIsNotValid();
    setOutputValues(std::vector{ value });
  }

  size_t LineConfig::getNumConfiguredOffsets() const {
    throwIfIsNotValid();
    return gpiod_line_config_get_num_configured_offsets(line_config);
  }

  std::vector<unsigned int> LineConfig::getConfiguredOffsets(size_t lines_number) const {
    auto offsets = std::make_unique<unsigned int[]>(lines_number);
    size_t size = gpiod_line_config_get_configured_offsets(line_config, offsets.get(), lines_number);
    std::vector<unsigned int> result(size);
    for (size_t index = 0; index < size; index++) {
      result[index] = offsets[index];
    }
    return result;
  };
}
