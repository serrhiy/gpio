#pragma once

#include "LineSettings.hh"
#include "LineInfo.hh"

#include <gpiod.h>
#include <ranges>
#include <concepts>
#include <algorithm>
#include <vector>

namespace gpio {
  template<typename T>
  concept UnsignedIntRange = 
    std::ranges::range<T> && 
    std::convertible_to<std::ranges::range_value_t<T>, unsigned int>;

  template<typename T>
  concept LineValueRange = 
    std::ranges::range<T> &&
    std::same_as<std::ranges::range_value_t<T>, LineValue>;

  class LineConfig {    
    gpiod_line_config* line_config;

    void free();
    bool isValid() const;
    void throwIfIsNotValid() const;

    public:
    LineConfig();

    LineConfig(const LineConfig&) = delete;
    LineConfig& operator=(const LineConfig&) = delete;

    LineConfig(LineConfig&& other) noexcept;
    LineConfig& operator=(LineConfig&& other) noexcept;

    ~LineConfig();

    void reset() const;

    void addLineSettings(const UnsignedIntRange auto& offsets, LineSettings& line_settings) const {
      using std::ranges::begin, std::ranges::end;

      throwIfIsNotValid();
      std::vector<unsigned int> data(begin(offsets), end(offsets));
      int result = gpiod_line_config_add_line_settings(
        line_config, data.data(), data.size(), line_settings.line_settings
      );
      if (result != 0) {
        throw std::runtime_error{ "Fail addLineSettings setOutputValues" };
      }
    }

    void addLineSettings(unsigned int offset, LineSettings& line_settingss) const;

    LineSettings getLineSettings(const unsigned int offset) const;
    
    void setOutputValues(const LineValueRange auto& values) const {
      using std::ranges::begin, std::ranges::end;

      throwIfIsNotValid();
      std::vector<gpiod_line_value> data;
      for (const auto& value: values) {
        data.push_back(line_value_reverse_map.at(value));
      }
      
      int result = gpiod_line_config_set_output_values(line_config, data.data(), data.size());
      if (result != 0) {
        throw std::runtime_error{ "Fail doing setOutputValues" };
      }
    }

    void setOutputValue(const LineValue value) const;

    size_t getNumConfiguredOffsets() const;
    std::vector<unsigned int> getConfiguredOffsets(size_t lines_number) const;
  };
}
