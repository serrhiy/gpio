#pragma once

#include "LineInfo.hh"
#include "LineConfig.hh"

#include <gpiod.h>
#include <string>
#include <vector>
#include <map>
#include <ranges>
#include <memory>

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