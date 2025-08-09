#pragma once

#include "LineConfig.hh"
#include "LineInfo.hh"

#include <gpiod.h>
#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

namespace gpio {
  class LineRequest {
    gpiod_line_request* line_request;

    bool isValid() const;
    void throwIfIsNotValid() const;
    void release();
  public:
    LineRequest() = delete;
    explicit LineRequest(gpiod_line_request* line_request);

    LineRequest(const LineRequest&) = delete;
    LineRequest& operator=(const LineRequest&) = delete;

    LineRequest(LineRequest&& other) noexcept;
    LineRequest& operator=(LineRequest&& other);

    ~LineRequest();

    std::string getChipName() const;
    size_t getNumRequestedLines() const;
    std::vector<unsigned int> getRequestedOffsets() const;
    LineValue getValue(unsigned int offset) const;

    std::map<unsigned int, LineValue>
      getValuesSubset(const std::vector<unsigned int>& offsets) const;
  };
}
