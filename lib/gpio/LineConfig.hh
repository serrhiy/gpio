#pragma once

#include "LineInfo.hh"
#include "LineSettings.hh"

#include <gpiod.h>
#include <vector>

namespace gpio {
  class LineRequest;

  class LineConfig {
    friend class LineRequest;

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

    void addLineSettings(
      const std::vector<unsigned>& offsets, LineSettings& line_settings
    ) const;

    void addLineSettings(
      const unsigned offset, LineSettings& line_settingss
    ) const;

    LineSettings getLineSettings(const unsigned offset) const;

    void setOutputValues(const std::vector<LineValue>& values) const;

    void setOutputValue(const LineValue value) const;

    size_t getNumConfiguredOffsets() const;
    std::vector<unsigned int> getConfiguredOffsets(size_t lines_number) const;
  };
}
