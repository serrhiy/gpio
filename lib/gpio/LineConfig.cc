#include "LineConfig.hh"
#include "LineSettings.hh"

#include <gpiod.h>
#include <stdexcept>
#include <memory>
#include <ranges>

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

  void LineConfig::addLineSettings(const std::vector<unsigned>& offsets, LineSettings& line_settings) const {
    throwIfIsNotValid();
    int result = gpiod_line_config_add_line_settings(line_config, offsets.data(), offsets.size(), line_settings.line_settings);
    if (result == -1) {
      throw std::runtime_error{ "addLineSettings error: " + std::to_string(result) };
    }
  }

  void LineConfig::addLineSettings(const unsigned offset, LineSettings& line_settings) const {
    throwIfIsNotValid();
    return addLineSettings(std::vector{ offset }, line_settings);
  }

  LineSettings LineConfig::getLineSettings(const unsigned offset) const {
    throwIfIsNotValid();
    gpiod_line_settings* line_settings = gpiod_line_config_get_line_settings(line_config, offset);
    return LineSettings{ line_settings };
  }

  void LineConfig::setOutputValues(const std::vector<LineValue>& values) const {
    using std::views::transform;

    const auto view = values
      | transform([](const auto& value) { return line_value_reverse_map.at(value); });
    std::vector<gpiod_line_value> data(view.begin(), view.end());
    
    int result = gpiod_line_config_set_output_values(line_config, data.data(), data.size());
    if (result == -1) {
      throw std::runtime_error{ "setOutputValues error: " + std::to_string(result) };
    }
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
